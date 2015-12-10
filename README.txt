||======================================================||
||					CONTROLS							||
||======================================================||
	Movement:
		- W = Move Forward
		- S = Move Down
		- A = Move Left
		- D = Move Right
	Switches:
		- J = Shadows
		- K = Geometry
		- L = Lights
	Ect:
		- Esc = close program
		
||======================================================||
||					POST MORTEM							||
||======================================================||

	The goal of nsfwgl (not suitable for work graphics library), and the defer project included, was to create a basic framework on which to make 
	a basic graphics framework and to "demonstrate knowledge of graphics programming and the functionality of modern Graphics Programming".
	Necessary pieces of evidence needed to demonstrate knowledge include the use of deferred rendering, asset management, a GPU based
	particle emitter, and a thorough game state. The project that I have created includes all of the necessary pieces of evidence.
	
	Many challenges were faced during my work on nsfwgl. Major challenges include Getting deferred rendering set up and running and understanding
	how GPU based particles work. These were particularly difficult for me for when they were first introduced and talked about I didn't fully grasp
	the concept. Its not until I attempted them several times each that I was finally understanding how each worked. What also helped me get to understand
	these concepts was talking to other classmates about it constantly.
	
	Events that I consider triumphs would have to be whenever I got the next part of nsfwgl working properly. For me graphics programming is not a topic
	in which I excel so every working part is a victory for me.
	
	Speaking of graphics programming not being my strong suit, it's time to talk about failures. I have a plentiful list of failures from this project but
	i'll mainly talk about what made me ask for outside help, First one was when I was trying to get deferred rendering up and running. All I was getting for
	awhile was clear color and I couldn't figure out why. I asked for some help and my classmate Jeff ended up going through my code and leaving breadcrumbs
	of things I needed to do/ fix in order to get everything to work. Which helped a ton and got it so I understood deferred rendering a bit better. The next
	big issue I had was towards the end of the project when we started working GPU Particle emitters. The addition of geometry shaders threw me off at first.
	That lead me to look at Jeffs code and see how it works. After figuring out how it works I edited my code to how it was supposed to be.
	
	The only thing i've used in this project that I think counts as an external tool would be Nsight. This tool is a graphics debugger from Nividia that allows
	you to capture a frame and examine it. You can see every step in the rendering process, all of the textures that have been loaded, and all the buffers. As
	for external help i'd have to say the biggest help was my classmate Jeff, but everyone in the class helped in one way or another. Everyone helps everyone 
	else in the class as they can and we were all working on the same project so in all reality, each of these projects is a coalition of everyones work.
	Major help for the start of this project however goes to the teacher Esme for creating the base that we all worked off of.
	
	For personal growth in the future, I might take this project and attempt it aggain on my own starting from the ground up. But this would only be if I really
	need my own framework or if i'm really bored. For I can tell one thing about my career; I really don't want to be a graphics programmer.