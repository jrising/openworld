openworld
=========

OpenWorld Modeling Framework

# Introduction

OpenWorld is a framework to make modeling simple and reliable, while
leaving control in the hands of the developers.  It is built upon the
OpenDims dimension framework (https://github.com/jrising/opendims),
providing a basis for unit checking throughout the system.

## Components

The elements of OpenWorld can be selectively used, based on the kind
of model being built.  Although there are a number of
cross-dependencies, the broad components are as follows.

* `datastr`: Objects for resolution-invariant numerical modeling.
  These allow one to define, for example, a `TemporalGeographicMap`
  describing a spatiotemporal precipitation field, and then perform
  mathematical operations on the entire field, and compute results
  that combine it with another `TemporalGeographicMap` at a different
  resolution.

* `multi`: A Bayesian model coupling system, which allows (1) models
  to be coupled without recalibration of the component models, (2)
  multiple models to simultaneously inform a single variable, and (3)
  models describing different scales to couple across the scales.

* `sysdyn`: A framework for system dynamics, which allows the entire
  model to be described in equations, and producing variables which
  themselves have dependencies with the other variables in the system.

* `memory`: Tools for serialization and memory management.

* `model`: Tools for nonlinear optimization of model parameters, using
  (amongst other methods) genetic algorithms.

# Examples:

## HydroNet

The most advanced example of a model using the `datastr` framework is
the `hydronet`, a physically-based, high-resolution hydrological
model.  It is available at https://github.com/jrising/hydronet

## SolowModel

A simple example of the system dynamics framework, through the famous
Solow growth model.  However, this model has not been updated since
the C# version of OpenWorld.  See
https://github.com/jrising/openworld/tree/master/examples/sample/SolowModel.cpp

## Environmental Conflict model

The canonical example of the model coupling framework is the
environmental conflict example, in
https://github.com/jrising/openworld/tree/master/examples/coupling.
It includes validation of the method and cross-scale coupling.
