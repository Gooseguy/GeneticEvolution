GeneticEvolution
Christian Cosgrove, 2014

Using a genetic algorithm, this program generates a soft-body creature capable of walking, jumping, or performing another task (dependent on optimizing a performance function).

Agents consist of a network of spring-connected nodes.  The springs can extend or contract periodically, allowing for motion.

This program includes a configuration.txt file.  This file allows the user to easily modify program parameters without modifying source/recompiling.  When modifying this file, be sure there are no spaces after colons and no extraneous characters.

The following parameters are available in configuration.txt:
F WindowWidth: // The width of the window.
F WindowHeight: // The height of the window
B DepthTesting: // Whether to enable depth testing.  This can slow down some GPU-bottlenecked machines.
B Multisampling: // Whether to enable multisampling. Like depth testing, this parameter is of consideration for low-end GPUs.
I MultisampleBuffers://The number of duplicate buffers for use in multisampling.  More means better antialiasing quality but slower rendering.
I MultisampleSamples://The number of times to perform multisampling.  More means better quality.
I NumAgents: //The population size.  Larger sizes reduce genetic drift, but take longer to yield similar results.
I AgentCubeWidth: //The (integer) width of the initial cube configuration for the agent population.
F AgentNodeSpacing: //The spacing for the agents' initial cube configuration.
F TimeStep: //Simulation time step. Larger values speed up computation but lead to instability.  If you observe the agents "blowing up", decrease this value.
F GenerationLength: //Proportional to the length of time per generation.
F GravitationalAcceleration://Proportional to the strength of gravity.
F DefaultSpringConstant: //The default value for the agents' spring constant.  This does not typically change.
F FieldOfView: // A larger FOV allows the user to see a larger scene, but leads to more distortion.
F ExtensionAmountVariance: // The mutation rate for the amount of muscle extension.
F ExtensionPeriodVariance: // The mutation rate for the period of muscle contraction/extension.  Making this nonzero can lead to movements becoming out of sync, so I suggest you leave it 0.
F ExtensionLengthVariance: // The mutation rate for the amount of time spent contracting/extending each muscle.
F ExtensionOffsetVariance: // The mutation rate for the phase shift of contraction/extension
F ExtensionAmountMaxMult: //The maximum ratio between the extended state and the relaxed state.
F DragCoefficient: // The coefficient of drag for the system.  a=F_other-kv
F NewAgentProbability: // The probability of introducing an unevolved agent into the population.  This functions to reset the simulation if a "dead end" is reached.
F GeometryMutationFactor: // The maximum mutation shift of a node's initial position
F RenderColorMult: // aesthetic : the factor determining the effect of body stresses on rendering
F DefaultNodeMass: // the mass of a node (this does not change)
F RemoveNodeProbability: // The probability of deleting a node as a mutation
F AddNodeProbability: // The probability of adding a node as a mutation
S AgentOutputFilename: // The file name for the agent storage output function.  Press P to save the selected agent to a file for future simulation and/or analysis.
B UseAgentOutputFile: //Whether to use the stored agent in AgentOutputFilename as a starting point for the next simulation.


The following controls are available:
WASD - camera movement
Mouse - camera rotation
T, G - change playback speed
up, down - manually choose selected agent
LMB - select nearest agent to camera
Escape - quit
