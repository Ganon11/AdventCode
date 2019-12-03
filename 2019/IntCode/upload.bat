del /Q dist\*
python setup.py sdist bdist_wheel
python -m twine upload --repository-url https://test.pypi.org/legacy/ dist/* --verbose
cd ..
python -m pip install -i https://test.pypi.org/simple/ IntCode-Ganon11==0.0.10