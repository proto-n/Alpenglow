import alpenglow.Getter as rs
import alpenglow as prs


class PopularityTimeframeModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        config = self.parameter_defaults(
            top_k=100,
            min_time=0,
            seed=67439852,
            out_file=None,
            filters=[],
            loggers=[],
        )

        model = rs.PopularityModel()
        updater = rs.PopularityTimeFrameModelUpdater(**self.parameter_defaults(
          tau=86400
        ))
        updater.set_model(model)

        model = model
        learner = [updater]

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
