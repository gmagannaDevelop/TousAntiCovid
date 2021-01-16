# bin/ directory

Usually this directory wouldn't be included in a repo, but **it must exist** in order
to build the executable (see `src/Makefile`).

## Minimal, barebones example :

```bash
./viral-simulation 30 80 0.20 0.10 0.05 10000
```

## Advanced useage :

You might want to investigate the effects of different combinations of parameters.

To do so, we provide a couple of scripts `runall.fish` and `killall.fish`. These scripts leverage the [fish shell](https://fishshell.com/). This shell language was preferred over bash as it is more human-readable and int the author's words "_Finally, a command line shell for the 90s. fish is a smart and user-friendly command line shell_". 



### Batch-executing :

You can run a batch of simulations with the param grid that we propose (it has no particular meaning) or one of your choice. Simply replace `param-grid.txt` with a file of your choice, just check how the parameters should be provided.

```bash
fish runall.fish param-grid.txt
```

### Stopping all the jobs from the batch :

```bash
fish killall.fish
```




