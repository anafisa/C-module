from distutils.core import setup, Extension

module1 = Extension(
      'probstat', # название модуля внутри Python
      sources = ['probstat.c'] # Исходные файлы модуля
)

setup(
      name = 'probstat', # название модуля (my_plus.__name__)
      version = '1.1', # версия модуля
      description = 'Probability & Statistics module', # описание модуля
      ext_modules= [module1] # объекты типа Extension (мы объявили его выше)
)