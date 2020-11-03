all: vendor examples test

vendor:
	bundle install --path vendor/bundle

examples: vendor
	bundle exec vendor/bundle/ruby/2.6.0/bin/arduino_ci_remote.rb --skip-unittest

test: vendor
	bundle exec vendor/bundle/ruby/2.6.0/bin/arduino_ci_remote.rb --skip-compilation
