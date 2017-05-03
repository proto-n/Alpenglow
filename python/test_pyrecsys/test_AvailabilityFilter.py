import alpenglow as prs
import alpenglow.Getter as rs
import pandas as pd


class TestAvailabilityFilter:
    def test_init(self):
        df = pd.DataFrame({'time': [30, 20, 10], 'item': [3, 2, 1], 'duration': [10, 10, 10]})
        a = prs.AvailabilityFilter(df)
        r = rs.RecDat()
        r.time = 21
        a.run(r)
