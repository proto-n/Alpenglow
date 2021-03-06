#include <gtest/gtest.h>
#include "../../main/models/factor/SvdppModel.h"
#include "../../main/models/factor/SvdppModelUpdater.h"
#include "../../main/models/factor/SvdppModelGradientUpdater.h"
#define DIMENSION 10
#define MAXUSER 13
#define MAXITEM 9

namespace {
class TestSvdppModel : public ::testing::Test { 
  public:
    vector<RecDat*> rec_dats;
    
    TestSvdppModel(){}
    virtual ~TestSvdppModel(){}
    virtual void SetUp(){
    }
    RecDat* create_rec_dat(int user, int item, double time, double score){
      RecDat* recDat = new RecDat;
      recDat -> user = user;
      recDat -> item = item;
      recDat -> time = time;
      recDat -> score = score;
      rec_dats.push_back(recDat);
      return recDat;
    }
    virtual void TearDown(){
      for (vector<RecDat*>::iterator it = rec_dats.begin();it!=rec_dats.end();it++){
        delete *it;
      }
    }
};
}

TEST_F(TestSvdppModel, test){
  vector<double> user_vector_weights={1.0,0.0,1.0,0.3};
  vector<double> history_weights={0.0,1.0,1.0,0.6};
  for(uint i=0;i<user_vector_weights.size();i++){
    double user_vector_weight=user_vector_weights[i];
    double history_weight=history_weights[i];
    SvdppModelParameters model_parameters;
    model_parameters.dimension=10;
    model_parameters.begin_min=0.1;
    model_parameters.begin_max=0.1;
    model_parameters.initialize_all=false;
    model_parameters.use_sigmoid=false;
    model_parameters.user_vector_weight=user_vector_weight;
    model_parameters.history_weight=history_weight;
    model_parameters.norm_type="exponential";
    model_parameters.gamma=0.8;
    SvdppModel model(&model_parameters);
    SvdppModelGradientUpdaterParameters grad_upd_parameters;
    grad_upd_parameters.learning_rate=0.1;
    grad_upd_parameters.cumulative_item_updates=false;
    SvdppModelGradientUpdater gradient_updater(&grad_upd_parameters);
    SvdppModelUpdater simple_updater;
    gradient_updater.set_model(&model);
    simple_updater.set_model(&model);
    EXPECT_TRUE(model.self_test());
    EXPECT_TRUE(gradient_updater.self_test());
    EXPECT_TRUE(simple_updater.self_test());
    for(int i=0;i<100;i++){
      create_rec_dat(i%MAXUSER,i%MAXITEM,i,1);
    }
    for(uint i=0;i<rec_dats.size();i++){
      model.add(rec_dats[i]);
      simple_updater.update(rec_dats[i]);
    }
    for(uint i=0;i<rec_dats.size();i++){
    //for(uint i=0;i<5;i++){
      double orig_pred = model.prediction(rec_dats[i]);
      double gradient = orig_pred - 1;
      gradient_updater.update(rec_dats[i],gradient);
      double new_pred = model.prediction(rec_dats[i]);
      if(orig_pred<1){
        EXPECT_GT(new_pred,orig_pred);
      }
      //cerr << "orig pred=" << orig_pred << " new pred=" << new_pred << endl;
    }
  }
}

int main (int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

