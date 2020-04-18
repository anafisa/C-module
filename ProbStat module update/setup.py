from distutils.core import setup, Extension

module1 = Extension(
	'ProbStat',
	sources = ['probstat_module.c'] 
)

setup(
	name = 'ProbStat',
	version = '1.0',
	description = 'Student assistant in the world of probability theory and mathematical statistics',
	ext_modules = [module1]
)
