class OfflineEvaluator /Abstract/{
  %TypeHeaderCode
  #include "offline_evaluators/OfflineEvaluator.h"
  %End
  public:
    OfflineEvaluator();
    virtual ~OfflineEvaluator();
    virtual void evaluate()=0;
    bool self_test();
};
