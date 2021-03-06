#include <gtest/gtest.h>
#include "../../main/models/baseline/TransitionProbabilityModel.h"
#include "../../main/models/baseline/TransitionProbabilityModelUpdater.h"
#include "../../main/loggers/TransitionModelEndLogger.h"


namespace{

class TestTransitionProbabilityModel : public ::testing::Test {
  public:
    TestTransitionProbabilityModel(){}
    virtual ~TestTransitionProbabilityModel(){}
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }
    RecDat create_recdat(int time,int user,int item,int score){
      RecDat rec_dat;
      rec_dat.time = time;
      rec_dat.user = user;
      rec_dat.item = item;
      rec_dat.score = score;
      return rec_dat;
    }
};
class TestTransitionEndLogger : public ::testing::Test {
  public:
    TestTransitionEndLogger(){}
    virtual ~TestTransitionEndLogger(){}
    virtual void SetUp(){
    }
    virtual void TearDown(){
      for(int i=0;i<rec_dat_container_.size();i++){
        delete rec_dat_container_[i];
      }
    }
    vector<RecDat*> rec_dat_container_;
    RecDat* create_recdat(int time,int user,int item,int score){
      RecDat* rec_dat = new RecDat;
      rec_dat->time = time;
      rec_dat->user = user;
      rec_dat->item = item;
      rec_dat->score = score;
      rec_dat_container_.push_back(rec_dat);
      return rec_dat;
    }
};

}

TEST_F(TestTransitionProbabilityModel, test){
  vector<RecDat> timeline;
  timeline.push_back(create_recdat(0,10,20,1));
  timeline.push_back(create_recdat(1,10,10,1));
  timeline.push_back(create_recdat(2,10,20,1));
  timeline.push_back(create_recdat(3,10,10,1));
  timeline.push_back(create_recdat(4,10,20,1));
  timeline.push_back(create_recdat(5,10,30,1));
  timeline.push_back(create_recdat(6,10,20,1));
  timeline.push_back(create_recdat(7,10,30,1));
  timeline.push_back(create_recdat(8,10,20,1));
  timeline.push_back(create_recdat(9,10,30,1));
  timeline.push_back(create_recdat(10,10,20,1));
  //statistics: a 20-as itemet 3-szor a 30-as, 2-szer a 10-es kovette
  timeline.push_back(create_recdat(10,20,20,1));
  //statistics: a 20-as itemet 3-szor a 10-es, 2-szer a 30-as kovette
  TransitionProbabilityModel model;
  TransitionProbabilityModelUpdaterParameters params;
  params.filter_freq_updates = false; //default: do not do any filtering
  TransitionProbabilityModelUpdater updater(&params);
  updater.set_model(&model);
  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(updater.self_test());
  RecDat rec_dat;
  rec_dat = create_recdat(30,10,30,1);
  EXPECT_EQ(0, model.prediction(&rec_dat));
  rec_dat = create_recdat(30,20,30,1);
  EXPECT_EQ(0, model.prediction(&rec_dat));
  for(RecDat rec_dat : timeline){
    updater.update(&rec_dat);
  }
  rec_dat = create_recdat(30,10,30,1);
  EXPECT_EQ(log(3+1), model.prediction(&rec_dat));
  rec_dat = create_recdat(30,20,30,1);
  EXPECT_EQ(log(3+1), model.prediction(&rec_dat));
  rec_dat = create_recdat(30,10,10,1);
  EXPECT_EQ(log(2+1), model.prediction(&rec_dat));
  rec_dat = create_recdat(30,20,10,1);
  EXPECT_EQ(log(2+1), model.prediction(&rec_dat));
}

TEST_F(TestTransitionEndLogger, test){
  vector<RecDat> timeline;
  TransitionProbabilityModel model;
  TransitionProbabilityModelUpdaterParameters params;
  params.filter_freq_updates = false; //default: do not do any filtering
  TransitionProbabilityModelUpdater updater(&params);
  updater.set_model(&model);
  EXPECT_TRUE(model.self_test());
  EXPECT_TRUE(updater.self_test());
  TransitionModelEndLoggerParameters logger_params;
  logger_params.max_length=5;
  TransitionModelEndLogger logger(&logger_params);
  logger.set_model(&model);
  PopContainer pop_container;
  logger.set_pop_container(&pop_container);
  EXPECT_TRUE(logger.self_test());
  pop_container.increase(2);
  pop_container.increase(2);
  pop_container.increase(2);
  pop_container.increase(2); //4x
  pop_container.increase(3);
  pop_container.increase(3); //2x
  pop_container.increase(5);
  pop_container.increase(5);
  pop_container.increase(5);
  pop_container.increase(5);
  pop_container.increase(5);
  pop_container.increase(5); //6x
  RecDat* rec_dat;
  rec_dat = create_recdat(1,1,1,1);
  updater.update(rec_dat);
  for(int i=0;i<10;i++){
    rec_dat = create_recdat(1,1,2,1);
    updater.update(rec_dat);
    rec_dat = create_recdat(1,1,3,1);
    updater.update(rec_dat);
  }
  rec_dat = create_recdat(1,1,1,1);
  updater.update(rec_dat);
  for(int i=0;i<10;i++){
    rec_dat = create_recdat(1,1,4,1);
    updater.update(rec_dat);
    rec_dat = create_recdat(1,1,5,1);
    updater.update(rec_dat);
    rec_dat = create_recdat(1,1,6,1);
    updater.update(rec_dat);
  }
  rec_dat = create_recdat(1,1,1,1);
  updater.update(rec_dat);
  for(int i=0;i<10;i++){
    RecDat* rec_dat_1 = create_recdat(1,1,7,1);
    RecDat* rec_dat_2 = create_recdat(1,1,i,1);
    for(int j=0;j<i;j++){
      updater.update(rec_dat_1);
      updater.update(rec_dat_2);
    }
  }
  std::stringstream ss;
  logger.run_core(ss);
  vector<string> expected_lines;
  expected_lines.push_back("0 0 0");
  expected_lines.push_back("1 3 0 7,2 2,1 4,1");
  expected_lines.push_back("2 2 4 3,10 7,2");
  expected_lines.push_back("3 3 2 2,9 7,3 1,1");
  expected_lines.push_back("4 2 0 5,10 7,4");
  expected_lines.push_back("5 2 6 6,10 7,5");
  expected_lines.push_back("6 3 0 4,9 7,6 1,1");
  expected_lines.push_back("7 9 0 7,14 9,9 8,8 6,6 5,5");// 4,4 3,3 2,2 1,1");
  expected_lines.push_back("8 1 0 7,8");
  expected_lines.push_back("9 1 0 7,8"); 
  for(int i=0;i<expected_lines.size();i++){
    string actual_line;
    std::getline(ss,actual_line);
    EXPECT_EQ(expected_lines[i],actual_line);
  } 
  //cerr << ss.str() << endl;
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
