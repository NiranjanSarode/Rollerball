import random

# Define the size of the population
population_size = 16

# Define the number of generations
generation_count = 20

# Define the number of arrays and the range for coefficient values (0 to 100)
num_arrays = 4
array_length = 49
min_value = 0
max_value = 100
mutation_rate = 0.1

# Define the Rollerball game interface
def play_rollerball(population):
    # print coefficients
    for i in range(len(population)):
        print(f"Array {i}:")
        for j in range(len(population[i])):
            print(f"    {population[i][j]}")
            #print reverse of population ij
            print(f"    {population[i][j][::-1]}")
    # Interface with your Rollerball game here
    # You should provide coefficients to the game, play a match, and return the result
    # Replace this with actual game logic
    #input results from user
    result1 = input()
    result2 = input()
    
    return [int(result1), int(result2)]

# Initialize the population with random coefficients for each array
population = []
for _ in range(population_size):
    individuals = [[random.randint(min_value, max_value) for _ in range(array_length)] for _ in range(num_arrays)]
    population.append(individuals)

# Genetic algorithm optimization
for generation in range(generation_count):
    print(f"Generation {generation + 1}:")

    # Evaluate the population
    scores = play_rollerball(population)

    # Select the top-performing individuals
    sorted_population = [population[scores[0]], population[scores[1]]]
    top_individuals = sorted_population
    print(sorted_population[0])
    # Create a new generation through crossover and mutation
    new_population = []
    for _ in range(population_size):
        parents = random.sample(top_individuals, 2)
        children = []
        for i in range(num_arrays):
            parent1, parent2 = parents
            child = [(p1 + p2) // 2 for p1, p2 in zip(parent1[i], parent2[i])]
            # Apply mutation
            for j in range(array_length):
                if random.random() < mutation_rate:
                    child[j] = random.randint(min_value, max_value)
            children.append(child)
        new_population.append(children)

    population = new_population
    

# Print the final best coefficients
best_coefficients = sorted_population[0]
print("Best Coefficients:")
for i, coefficients in enumerate(best_coefficients):
    print(f"Array {i + 1}: {coefficients}")
