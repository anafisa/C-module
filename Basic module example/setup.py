from distutils.core import setup, Extension

module1 = Extension(
      'my_plus', # название модуля внутри Python
      sources = ['plus.c'] # Исходные файлы модуля
)

setup(
      name = 'my_plus', # название модуля (my_plus.__name__)
      version = '1.1', # версия модуля
      description = 'Simple module', # описание модуля
      ext_modules= [module1] # объекты типа Extension (мы объявили его выше)
)