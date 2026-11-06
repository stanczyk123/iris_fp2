from setuptools import find_packages
from setuptools import setup

setup(
    name='iris_msgs',
    version='0.0.0',
    packages=find_packages(
        include=('iris_msgs', 'iris_msgs.*')),
)
