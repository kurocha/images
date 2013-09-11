
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "0.8.0"

define_project "Dream Imaging" do |project|
	project.add_author "Samuel Williams"
	project.license = "MIT License"
	
	project.version = "0.1.0"
end

define_target "dream-imaging" do |target|
	target.build do |environment|
		build_directory(package.path, 'source', environment)
	end
	
	target.depends "Library/Dream"
	target.depends "Language/C++11"

	target.depends "Library/png"
	target.depends "Library/jpeg"
	
	target.depends "Library/Euclid"
	
	target.provides "Library/Dream/Imaging" do
		append linkflags "-lDreamImaging"
	end
end

define_target "dream-imaging-tests" do |target|
	target.build do |environment|
		build_directory(package.path, 'test', environment)
	end
	
	target.run do |environment|
		run_executable("bin/dream-imaging-test-runner", environment)
	end
	
	target.depends "Library/Dream/Imaging"
	target.depends "Library/UnitTest"
	
	target.provides "Test/Dream/Imaging"
end

define_configuration "dream-imaging" do |configuration|
	configuration.public!
	
	configuration.import! "dream"
	
	configuration.require "png"
	configuration.require "jpeg"
end

define_configuration "travis" do |configuration|
	configuration[:source] = "https://github.com/dream-framework"

	configuration.import! "platforms"
	configuration.import "dream-imaging"

	configuration[:run] = ["Test/Dream/Imaging"]
end
