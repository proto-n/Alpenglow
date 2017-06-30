import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math


class TestAsymmetricFactorModelExperiment:
    def test_AsymmetricFactorModelExperiment(self):
        experiment = alpenglow.experiments.AsymmetricFactorModelExperiment(
            top_k=100,
            seed=254938879,
            dimension=10,
            learning_rate=0.1,
            negative_rate=10
        )
        rankings = experiment.run("python/test_alpenglow/test_data_4", experimentType="online_id", verbose=True)
        assert rankings.top_k == 100
        desired_ranks = [101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 8, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 2, 101, 101, 2, 101, 101, 101, 101, 101, 101, 101, 18, 101, 101, 101, 101, 101, 101, 101, 101, 3, 101, 101, 101, 14, 2, 13, 101, 101, 2, 101, 16, 101, 101, 101, 101, 101, 101, 31, 101, 21, 21, 47, 101, 101, 101, 101, 101, 101, 48, 101, 10, 101, 50, 101, 101, 101, 2, 101, 101, 27, 101, 10, 55, 101, 101, 101, 101, 101, 101, 31, 101, 9, 2, 101, 22, 15, 50, 8, 101, 101, 55, 101, 101, 101, 7, 35, 57, 12, 101, 44, 64, 101, 101, 42, 3, 101, 101, 101, 59, 37, 42, 101, 33, 24, 42, 20, 1, 38, 5, 101, 35, 101, 101, 3, 39, 73, 101, 14, 53, 1, 47, 16, 64, 101, 40, 101, 101, 9, 101, 24, 80, 101, 7, 14, 35, 27, 63, 101, 21, 101, 101, 13, 101, 101, 101, 101, 74, 101, 4, 78, 101, 101, 101, 101, 64, 34, 84, 101, 101, 81, 101, 86, 101, 61, 74, 73, 32, 101, 101, 18, 8, 101, 1, 86, 75, 12, 31, 5, 101, 101, 101, 101, 8, 101, 78, 101, 9, 101, 42, 89, 101, 101, 47, 13, 32, 47, 101, 101, 74, 19, 35, 2, 101, 23, 101, 74, 58, 55, 97, 2, 36, 18, 101, 101, 101, 101, 101, 84, 27, 101, 28, 32, 63, 101, 8, 101, 32, 55, 22, 76, 101, 101, 27, 101, 75, 101, 9, 101, 68, 1, 58, 21, 1, 9, 101, 87, 57, 101, 101, 101, 101, 23, 85, 6, 101, 101, 84, 101, 99, 55, 101, 56, 46, 2, 22, 1, 101, 101, 101, 101, 84, 101, 84, 1, 101, 4, 89, 101, 101, 101, 101, 92, 14, 33, 42, 12, 46, 98, 32, 101, 101, 101, 101, 22, 101, 101, 101, 35, 101, 32, 101, 16, 101, 4, 101, 101, 72, 101, 15, 10, 101, 48, 16, 101, 84, 101, 101, 50, 21, 4, 88, 101, 4, 10, 101, 40, 101, 7, 101, 101, 101, 31, 68, 101, 101, 6, 23, 101, 101, 82, 75, 85, 71, 101, 101, 101, 1, 18, 101, 43, 77, 101, 8, 26, 101, 5, 85, 2, 101, 101, 101, 26, 16, 101, 101, 78, 27, 66, 72, 101, 101, 101, 101, 101, 101, 101, 101, 94, 101, 101, 101, 71, 101, 101, 8, 10, 101, 101, 101, 101, 101, 101, 101, 1, 16, 21, 30, 101, 101, 101, 72, 51, 101, 101, 101, 18, 101, 73, 26, 56, 101, 101, 4, 101, 12, 6, 101, 101, 20, 81, 11, 4, 14, 58, 4, 43, 74, 22, 101, 27, 2, 94, 101, 36, 78, 101, 12, 101, 43, 20, 101, 101, 65, 101, 40, 22, 101, 60, 101, 3, 101, 12, 101, 17, 101, 101, 101, 101, 101, 101, 20, 101, 101, 101, 101, 33, 101, 101, 69, 21, 8, 54, 101, 101, 12, 93, 101, 47, 42, 14, 56, 11, 50, 101, 62, 101, 7, 101, 3, 77, 3, 1, 3, 22, 1, 59, 44, 101, 27, 7, 5, 59, 81, 101, 101, 2, 101, 31, 19, 101, 101, 101, 101, 101, 17, 101, 11, 101, 46, 18, 101, 101, 14, 32, 43, 15, 101, 36, 12, 9, 4, 7, 21, 101, 30, 101, 101, 70, 6, 101, 101, 101, 101, 90, 101, 101, 101, 4, 12, 24, 3, 101, 7, 101, 101, 51, 101, 101, 97, 12, 101, 101, 55, 24, 96, 15, 101, 101, 2, 101, 85, 101, 11, 101, 41, 45, 80, 9, 101, 101, 18, 101, 101, 77, 101, 70, 48, 68, 101, 54, 101, 9, 101, 101, 101, 8, 37, 101, 101, 100, 50, 70, 11, 101, 82, 101, 73, 16, 89, 66, 39, 1, 101, 101, 14, 101, 101, 101, 101, 101, 70, 101, 101, 101, 91, 56, 101, 101, 101, 101, 79, 101, 51, 101, 101, 86, 10, 67, 101, 33, 5, 3, 2, 2, 101, 101, 5, 15, 101, 35, 54, 1, 101, 101, 1, 2, 101, 42, 101, 101, 17, 101, 101, 1, 101, 1, 101, 101, 3, 56, 17, 93, 15, 54, 34, 49, 101, 101, 98, 51, 101, 21, 101, 35, 101, 101, 42, 74, 95, 1, 101, 101, 101, 101, 31, 85, 12, 101, 4, 6, 2, 101, 67, 48, 101, 82, 21, 52, 78, 91, 59, 101, 94, 101, 98, 101, 46, 74, 52, 101, 101, 93, 21, 2, 9, 101, 11, 4, 101, 101, 63, 101, 4, 101, 101, 101, 26, 42, 2, 101, 12, 101, 101, 101, 28, 101, 13, 101, 73, 29, 101, 93, 4, 1, 12, 101, 42, 101, 38, 47, 101, 3, 24, 81, 86, 101, 1, 94, 101, 1, 101, 37, 33, 89, 39, 24, 7, 74, 3, 101, 101, 50, 3, 59, 36, 7, 80, 25, 58, 30, 101, 3, 20, 1, 101, 101, 101, 101, 101, 87, 25, 75, 101, 66, 43, 1, 70, 101, 42, 32, 101, 101, 101, 101, 101, 101, 5, 10, 65, 48, 101, 101, 81, 32, 44, 5, 4, 101, 8, 8, 101, 101, 101, 101, 101, 101, 6, 68, 101, 101, 101, 101, 101, 18, 68, 101, 32, 11, 13, 101, 101, 38, 44, 6, 101, 27, 3, 16, 40, 55, 14, 2, 101, 101, 27, 32, 101, 101, 101, 16, 101, 101, 72, 101, 31, 75, 62, 101, 101, 101, 76, 70, 10, 6, 20, 101, 101, 6, 101, 29, 35, 101, 101, 41, 14, 38, 101, 101, 4, 101, 12, 101, 90, 6, 1, 54, 64, 101, 101, 101, 101, 101, 9, 101, 73, 101, 44, 34, 1, 22, 5, 69, 6, 30, 101, 4, 101, 24, 17, 101, 29, 23, 101, 16, 8, 45, 5, 90, 30, 9, 1, 101, 70, 101, 101, 27, 1, 75, 3, 101, 55, 17, 101, 7, 101, 18, 43, 27, 2, 101, 1, 6, 101, 3, 13, 17, 46, 71, 101, 101, 101]
        assert list(rankings["rank"].fillna(101)) == desired_ranks

