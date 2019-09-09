import configparser
from slave import Slave
from component import Component
from instrument import Instrument


def parse_slave(config_path):
    """Returns a dictionary of `Slave` objects."""
    config = configparser.ConfigParser()
    config.read(config_path)
    return {
        name: Slave.factory(config[name])
        for name in config.sections()
    }


def parse_component(config_path):
    """Returns a dictionary of `Component` objects."""
    config = configparser.ConfigParser()
    config.read(config_path)
    return {
        name: Component.from_config(config[name])
        for name in config.sections()
    }


def parse_instrument(config_path):
    """Returns a dictionary of `Instrument` objects.
    Instrument components haven't been mapped.
    """
    config = configparser.ConfigParser()
    config.read(config_path)
    return {
        name: Instrument.from_config(config[name])
        for name in config.sections()
    }
