import alpenglow.Getter as rs
import alpenglow as prs


class FactorModelExperiment(prs.OnlineExperiment):
    def config(self, elems):
        proceeding_logger = rs.ProceedingLogger()
        proceeding_logger.set_data_iterator(elems['recommender_data_iterator'])
        config = self.parameterDefaults(
            top_k=100,
            min_time=0,
            seed=0,
            out_file=None,
            filters=[],
        )
        config['loggers'] = [proceeding_logger] if self.verbose else []

        model = rs.FactorModel(**self.parameterDefaults(
            begin_min=-0.01,
            begin_max=0.01,
            dimension=10,
            initialize_all=False,
        ))

        updater = rs.FactorModelGradientUpdater(**self.parameterDefaults(
            learning_rate=0.05,
            regularization_rate=0.0
        ))
        updater.set_model(model)

        learner = rs.ImplicitGradientLearner()
        learner.set_train_matrix(elems['train_matrix'])
        learner.add_gradient_updater(updater)
        learner.set_model(model)

        negativeSampleGenerator = rs.UniformNegativeSampleGenerator(**self.parameterDefaults(
            negative_rate=0.0,
            initialize_all=False,
            seed=0,
        ))
        negativeSampleGenerator.set_train_matrix(elems['train_matrix'])
        negativeSampleGenerator.set_items(elems['items'])
        learner.set_negative_sample_generator(negativeSampleGenerator)

        pointWise = rs.ObjectiveMSE()
        gradientComputer = rs.GradientComputerPointWise(pointWise)
        gradientComputer.set_model(model)
        learner.set_gradient_computer(gradientComputer)

        return {
            'config': config,
            'model': model,
            'learner': learner
        }
