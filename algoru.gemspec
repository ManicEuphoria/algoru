# frozen_string_literal: true

require_relative 'lib/algoru/version'

Gem::Specification.new do |spec|
  spec.name = 'algoru'
  spec.version = Algoru::VERSION

  spec.required_ruby_version = Gem::Requirement.new('>= 2.3.0')

  spec.license = 'AGPL-3.0-only'

  spec.authors = [
    'taotsi',
    'EUPHORAY'
  ]
  spec.email = [
    'tmkf.guthrie@gmail.com',
    'weirdhoney.mizar@gmail.com'
  ]

  spec.summary       = 'a gem to implement algorithm and data structure in ruby language.'
  spec.description   = "don't skip this or this will be left what it is like now."
  spec.homepage      = 'https://github.com/ManicEuphoria/algoru'

  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  # spec.executables   = 'algoru'
  spec.bindir = 'bin'
  spec.require_paths = ['lib']
  spec.files = `git ls-files bin lib *.md LICENSE`.split("\n")
  p spec.files
  spec.metadata['source_code_uri'] = 'https://github.com/ManicEuphoria/algoru'
end
