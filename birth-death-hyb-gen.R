library(SiPhyNetwork)

##First we need a function that describes how inheritance probabilities are drawn
inheritance.fxn <- make.beta.draw(10,10)


##We also want to set the proportion of each type of hybrid event
hybrid_proportions <-c(0.5,  ##Lineage Generative
                       0.25, ##Lineage Degenerative
                       0.25) ##Lineage Neutral
##We can simulate to 7 extant tips with the SSA
ssa_nets<-sim.bdh.taxa.ssa(n=7,numbsim=1000,
                            lambda=1,mu=0.2,
                            nu=0.20, hybprops = hybrid_proportions,
                            hyb.inher.fxn = inheritance.fxn)

for(i in 1:1000)
    # Make sure that this network isn't empty
    if(!is.numeric(ssa_nets[[i]]))
        write(write.net(ssa_nets[[i]]), file='newick-strings.txt', append=T)