%Student Name: Omkar Acharya
%Student ID: 200155609
%Problem statement: Write a prolog style dataset of facts and rules for the given information

%Creatures come in two types: humans and birds.
edge(human, ako, creature).
edge(bird, ako, creature).

%One type of human is a man.
edge(man, ako, human).

%One type of bird is a turkey.
edge(turkey, ako, bird).

%Louis is a man.
edge(louis, isa, man).

%Albert is a man.
edge(albert, isa, man).

%Frank is a turkey.
edge(frank, isa, turkey).

%Humans normally have two legs
property(human, legs, two).

%Birds can normally fly
property(bird, fly, yes).

%Louis has one leg
property(louis, legs, one).

%Turkeys cannot fly.
property(turkey, fly, no).

%rel(SourceNode, RelationshipType, DestinationNode)
rel(A, Slot, B):- edge(A, Slot, B).
rel(A, Slot, B):- edge(A, Slot, Z), rel(Z, isa, B).
rel(A, Slot, B):- edge(A, Slot, Z), rel(Z, ako, B).

%overrides the default property if there exists any local attribute value
hasProperty(A, Prop, Value):- property(A, Prop, Value).
hasProperty(A, Prop, Value):- edge(A, isa, Z), hasProperty(Z, Prop, Value), \+ property(A, Prop, _). 
hasProperty(A, Prop, Value):- edge(A, ako, Z), hasProperty(Z, Prop, Value), \+ property(A, Prop, _). 