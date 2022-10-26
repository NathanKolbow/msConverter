---
title: 'Gala: A Python package for galactic dynamics'
tags:
  - C++
  - phylogenetics
  - network
  - ms
  - Newick
authors:
  - name: Nathan Kolbow
    orcid: 0000-0000-0000-0000
    equal-contrib: true
    affiliation: "1, 2"
  - name: Claudia Solis-Lemus
    equal-contrib: false
    affiliation: "1, 2"
affiliations:
 - name: Department of Statistics, University of Wisconsin-Madison, USA
   index: 1
 - name: Wisconsin Institute for Discovery, USA
   index: 2
date: 26 October 2022
bibliography: paper.bib
---

# Summary

Many biologists are interested in how different species have evolved throughout time. To this end, many different methods have been developed that take various forms of data in order to estimate evolutionary relationships between species of interest. These methods need to be tested for efficiency and accuracy. This is difficult to do with real biological data, both because we rarely know the true evolutionary relationship of the modeled species and because this data is near-impossible to procure in the abundance needed for sufficient testing. Simulation software have been developed that (1) provide data with known ground truths in sufficient abundance for testing, and (2) can be utilized by researchers to answer various other questions pertaining to evolutionary biology.

Another aspect of this research is the representation of these trees and networks. Diagrams are very illustrative, but unrealistic for most software. Thus, clearly there should be some way to store these trees and networks as strings, so that they are readily accessible by researchers and can be easily parsed by software. There are several ways of doing this, but the predominant method for representing trees and networks in phylogenetics currently is via the Newick format.

# Statement of need

One of the most popular software for these simulations in called `ms` [@Hudson:2001]. There are many things that this software can do which other existing simulators cannot, and it is apart of many researchers' software pipelines. Despite its popularity, though, the software predates the wide adoption of the Newick format by the phylogenetics community. Because of this, `ms` has its own format for expressing trees and networks which is entirely different from the Newick format. This software `msConverter` provides an easy mechanism via which researchers can converter between the format accepted by `ms` and the Newick format, instead of having to do so manually.

In addition to reducing time researchers must take manually converting between each format, we also anticipate that this software will help bridge the gap between `ms` and other phylogenetic software. Many software output phylogenetic trees and networks in the Newick format ((citations: hybrid-Lambda, ...)) and it is often infeasible to convert every tree or network output from these software to the format supported by `ms` by hand, so researchers are forced to utilize other, more restrictive alternatives. This software alleviates the need to use these alternatives.

Additionally, within this software we provide a ready-to-use framework for bifurcating Networks via `Network.cpp` and `Network.hpp`. When beginning development on a new project, it is common for researchers to re-write this code from scratch, so we hope that providing this code publicly will cut back on this upfront development cost.

# Acknowledgements

We acknowledge support from the Wisconsin Institute for Discovery throughout the development of this project.

# References