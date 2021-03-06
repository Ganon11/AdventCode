import setuptools

with open("README.md", "r") as fh:
  long_description = fh.read()

setuptools.setup(
  name="intcode-Ganon11",
  version="4.0.0",
  author="Ganon11",
  author_email="stark3@gmail.com",
  description="An interpreter for Advent of Code 2019's IntCode",
  long_description=long_description,
  url="https://github.com/Ganon11/AdventCode",
  packages=setuptools.find_packages(),
  install_requires=['defaultlist'],
  classifiers=[
    "Programming Language :: Python :: 3",
    "License :: OSI Approved :: MIT License",
    "Operating System :: OS Independent",
  ]
)
