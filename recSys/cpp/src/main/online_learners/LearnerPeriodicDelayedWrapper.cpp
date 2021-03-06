#include "LearnerPeriodicDelayedWrapper.h"


void LearnerPeriodicDelayedWrapper::update(RecDat* rec_dat){
  if(start_time_ == -1){
    start_time_ = rec_dat->time;
  }

  if(delay_ > 0){
    delay_accumulator_.push(rec_dat);
    RecDat* front = delay_accumulator_.front();
    while(!delay_accumulator_.empty() && front->time <= (rec_dat->time - delay_)){
      delayed_learn(front);
      delay_accumulator_.pop();
      front = delay_accumulator_.front();
    }
  } else {
    delayed_learn(rec_dat);
  }
}

void LearnerPeriodicDelayedWrapper::delayed_learn(RecDat* rec_dat){
  if(period_ > 0){
    int this_period_num = (rec_dat->time - start_time_)/period_;
    if(this_period_num == last_period_num_){
      period_accumulator_.push_back(rec_dat);
    } else {
      last_period_num_ = this_period_num;
      periodic_learn(period_accumulator_);
      period_accumulator_.clear();
      period_accumulator_.push_back(rec_dat);
    }
  } else {
    simple_learn(rec_dat);
  }
}

void LearnerPeriodicDelayedWrapper::periodic_learn(vector<RecDat*> recdats){
  for(RecDat* r : recdats){
    simple_learn(r);
  }
}

void LearnerPeriodicDelayedWrapper::simple_learn(RecDat* rec_dat){
  wrapped_learner_->update(rec_dat);
}

void LearnerPeriodicDelayedWrapper::set_wrapped_learner(Updater* learner){
  wrapped_learner_ = learner;
}
