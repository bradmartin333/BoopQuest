import glob
import os
import sys
from PIL import Image


def find_matching_files(directory, filename_pattern):
    """Finds all files in the given directory that match the filename pattern.

    Args:
        directory (str): The directory to search in.
        filename_pattern (str): The filename pattern to match, using wildcards.

    Returns:
        list: A list of full paths to the matching files.
    """

    full_path_pattern = os.path.join(directory, filename_pattern)
    matching_files = glob.glob(full_path_pattern)

    return matching_files


def load_png_images(directory, filename_pattern):
    """Finds all PNG images in the given directory and loads them.

    Args:
        directory (str): The directory to search in.
        filename_pattern (str): The filename pattern to match, using wildcards.

    Returns:
        list: A list of PIL Image objects representing the loaded PNG images.
    """

    png_images = []
    matching_files = find_matching_files(directory, filename_pattern)

    for file in matching_files:
        print(file)
        if file.lower().endswith(".png"):  # Check for PNG extension
            try:
                image = Image.open(file)
                image = image.convert("RGBA")
                png_images.append(image)
            except OSError:
                print(f"Error loading image: {file}")

    return png_images


def create_combined_image(png_images):
    """Creates a new PNG image by combining multiple PNG images horizontally.

    Args:
        png_images (list): A list of PIL Image objects representing the PNG images to combine.

    Returns:
        PIL Image: The combined image.
    """

    total_width = png_images[0].width * len(png_images)
    total_height = png_images[0].height

    combined_image = Image.new("RGBA", (total_width, total_height), (0, 0, 0, 0))
    x_offset = 0

    for image in png_images:
        combined_image.paste(image, (x_offset, 0), image)
        x_offset += image.width

    return combined_image


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python load_pngs.py <directory> <filename_pattern>")
        sys.exit(1)

    directory = sys.argv[1]
    filename_key = sys.argv[2]
    output_path = '../../Web/res/character/' + filename_key

    png_images = load_png_images(directory, '*' + filename_key + '*')

    if png_images:
        combined_image = create_combined_image(png_images)

        # Save the combined image with the specified format
        output_filename = f"{output_path}_{len(png_images)}.png"
        combined_image.save(output_filename)
        print(f"Combined image saved as {output_filename}")
    else:
        print("No PNG images found.")
