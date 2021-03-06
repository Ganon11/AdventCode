import setuptools

with open("README.md", "r") as fh:
  long_description = fh.read()

setuptools.setup(
  name="position-Ganon11",
  version="2.0.0",
  author="Ganon11",
  author_email="stark3@gmail.com",
  description="A library for tracking positions on a 2D coordinate system",
  long_description=long_description,
  url="https://github.com/Ganon11/AdventCode",
  packages=setuptools.find_packages(),
  classifiers=[
    "Programming Language :: Python :: 3",
    "License :: OSI Approved :: MIT License",
    "Operating System :: OS Independent",
  ]
)