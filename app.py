from flask import Flask, request, render_template, send_file, jsonify, send_from_directory
import os
import subprocess
from PIL import Image

app = Flask(__name__)

UPLOAD_FOLDER = 'uploads/'
OUTPUT_FOLDER = 'output/'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['OUTPUT_FOLDER'] = OUTPUT_FOLDER

# Ensure upload and output directories exist
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(OUTPUT_FOLDER, exist_ok=True)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/convert-to-png', methods=['POST'])
def convert_to_png():
    # Save the uploaded PPM image
    if 'image' not in request.files:
        return "No file part"
    file = request.files['image']
    if file.filename == '':
        return "No selected file"
    if file and file.filename.endswith('.ppm'):
        input_path = os.path.join(app.config['UPLOAD_FOLDER'], file.filename)
        file.save(input_path)

        # Remove .ppm extension and add .png
        png_filename = os.path.splitext(file.filename)[0] + '.png'
        output_png_path = os.path.join(app.config['OUTPUT_FOLDER'], png_filename)

        try:
            # Convert PPM to PNG for preview
            with Image.open(input_path) as img:
                img.save(output_png_path, 'PNG')

            # Check if the image was saved correctly
            if os.path.exists(output_png_path):
                print(f"Image saved successfully at {output_png_path}")
            else:
                print("Image conversion failed!")

            # Return the PNG file URL for the frontend to preview
            return f"/output/{png_filename}"
        except Exception as e:
            print(f"Error during image conversion: {e}")
            return "Error converting the file"

@app.route('/output/<filename>')
def serve_output_file(filename):
    return send_from_directory(app.config['OUTPUT_FOLDER'], filename)

@app.route('/upload', methods=['POST'])
def upload_image():
    # Save the uploaded image
    if 'image' not in request.files:  # Change 'file' to 'image'
        return jsonify({'error': "No file part"}), 400
    file = request.files['image']  # Change 'file' to 'image'
    if file.filename == '':
        return jsonify({'error': "No selected file"}), 400
    if file:
        input_path = os.path.join(app.config['UPLOAD_FOLDER'], file.filename)
        output_path = os.path.join(app.config['OUTPUT_FOLDER'], file.filename.rsplit('.', 1)[0] + '.ppm')
        file.save(input_path)

        # Prepare the command with the provided options
        command = ['./gammaCorr', input_path, '--output', output_path]

        # Get additional parameters from the form
        gamma_value = request.form.get('gamma')
        if gamma_value:
            command.extend(['--gamma', gamma_value])

        coeffs_value = request.form.get('coeffs')
        if coeffs_value:
            command.extend(['--coeffs', coeffs_value])

        version_value = request.form.get('version')
        if version_value:
            command.extend(['--version', version_value])

        benchmark_value = request.form.get('benchmark')
        if benchmark_value:
            command.extend(['--benchmark', benchmark_value])

        # Call the C program (gammaCorr) with the uploaded image and options
        subprocess.run(command)

        # Return the URL to download the processed image
        return jsonify({"processedImageUrl": f"/output/{file.filename.rsplit('.', 1)[0]}.png"})  # Adjusted the return to use the correct URL format

@app.route('/download/<filename>')
def download_image(filename):
    output_path = os.path.join(app.config['OUTPUT_FOLDER'], filename)  # Use filename in download path
    return send_file(output_path, as_attachment=True)

if __name__ == '__main__':
    app.run(debug=True)
