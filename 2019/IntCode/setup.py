import setuptools

with open("README.md", "r") as fh:
  long_description = fh.read()

setuptools.setup(
  name="intcode-Ganon11",
  version="0.0.10",
  author="Ganon11",
  author_email="stark3@gmail.com",
  description="An interpreter for Advent of Code 2019's IntCode",
  long_description=long_description,
  long_description_content_type="text/markdown",
  url="https://github.com/Ganon11/AdventCode",
  packages=setuptools.find_packages(),
  classifiers=[
    "Programming Language :: Python :: 3",
    "License :: OSI Approved :: MIT License",
    "Operating System :: OS Independent",
  ],
  python_requires='>=3.6'
)