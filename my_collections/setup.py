import setuptools

with open("README.md", "r") as fh:
  long_description = fh.read()

setuptools.setup(
  name="Ganon11-collections",
  version="1.0.0",
  author="Ganon11",
  author_email="stark3@gmail.com",
  description="A group of commonly used classes for Advent of Code",
  long_description=long_description,
  url="https://github.com/Ganon11/AdventCode",
  packages=setuptools.find_packages(),
  install_requires=[],
  classifiers=[
    "Programming Language :: Python :: 3",
    "License :: OSI Approved :: MIT License",
    "Operating System :: OS Independent",
  ]
)
