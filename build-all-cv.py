import os
import sys

CMD = 'xcodebuild -workspace %(work_space)s -scheme %(scheme)s -sdk %(sdk)s'
WORK_SPACE = 'HomageCV.xcworkspace'
SDKS = [
	'iphonesimulator8.0',
	'iphoneos8.0'
]
SCHEMES = [
	'Util',
	'Gpw',
	'MattingLib',
	'Image'
]


if __name__ == '__main__':

	work_space = WORK_SPACE

	for sdk in SDKS:
		for scheme in SCHEMES:
			cmd = CMD % locals()
			os.system(cmd)
			print cmd
