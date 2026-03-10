project('tworld-term', 'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=gnu23'])

libchips_proj = subproject('libchips')
libchips_dep = libchips_proj.get_variable('libchips_dep')
curses = dependency('curses')

sources = ['main.c', 'gameplay.c', 'misc.c', 'graphics.c']

tworld_term = executable('tworld_term',
                         sources,
                         dependencies: [libchips_dep, curses],
                         install : true
                        )

test('test', tworld_term)
