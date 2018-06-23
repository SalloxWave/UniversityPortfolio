
def intersect():
    return True
def min_bounding_box(box, box2):
    return True

from_box = [2,3,4,5]
from_punkt = from_box.upleftcorner()
to_punkt = [3,4]

moving_box = from_box

collition_box = [3,4,5,6]

from_and_to_box = min_bounding_box(from_punkt, to_punkt)

long_side = std::max(from_and_to_box.width(), from_and_to_box.height())

# Loop every step on the longest side
for i in range(long_side):
    # i = current step on long side
    moving_box.longside += 1
    moving_box.shortside += int(i*from_and_to_box.shortside/from_and_to_box.longside)

    if intersect(moving_box, collition_box):
        break
    else:
        from_box = moving_box

return moving_box



# =====

# Create a min_bounding_box of from and to, så att vi vet området där den ska flytta sig.

# Create a dummy_box with values thats going to be returned by this function
# Create a true_or_false variable to check if there is a collition or not, true_or_false = True

# Loop though the long side of the bounding box

    # i in loop is the step on the long side
    # then calculate how much to move on the short side with int(i*shortside/longside)

    # save a moved_box with the new values

    # if moved_box intersect with this.box
        # true_or_false = False
        # break out of the loop
    # else
        # dummy_box = moved_box


# if true_or_false == False    # Om det blev en kolition så ska vi returnera senaste fungerade kordinaterna
    # Return dummy_box with will be the last value that worked

# Return true_or_false








