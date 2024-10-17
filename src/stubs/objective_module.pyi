from __future__ import annotations
import numpy
__all__ = ['ObjectiveFunction']
class ObjectiveFunction:
    @staticmethod
    def _pybind11_conduit_v1_(*args, **kwargs):
        ...
    def __init__(self) -> None:
        ...
    def compute(self, arg0: numpy.ndarray[numpy.float64]) -> float:
        """
        Compute the objective value.
        """
    def get_call_count(self) -> int:
        """
        Get the current call count of the objective function
        """
