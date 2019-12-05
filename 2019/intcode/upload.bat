del /Q dist\*
python setup.py sdist bdist_wheel
python -m twine upload --repository-url https://test.pypi.org/legacy/ dist/* --verbose
cd ..
python -m pip install -i https://test.pypi.org/simple/ intcode-Ganon11==2.0.0