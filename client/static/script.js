document.addEventListener('DOMContentLoaded', function() {
    document.getElementById('fileInput').addEventListener('change', handleFileSelect, false);
});

function handleFileSelect(evt) {
    let files = evt.target.files;

    for (let i = 0, f; f = files[i]; i++) {
        // Check if the file is a .ppm image
        if (!f.name.endsWith('.ppm')) {
            alert('Error: Only .ppm image files are allowed.');
            // Clear the file input to reset the selection
            evt.target.value = ''; // Clear the file input
            throw new Error('Invalid file type selected. Only .ppm files are accepted.');
        }

        // Create FormData to send the .ppm file to the server for preview conversion (ppm to png)
        let formData = new FormData();
        formData.append('image', f);

        // Send the .ppm file to the server for conversion to .png for preview
        fetch('/convert-to-png', {
            method: 'POST',
            body: formData
        })
            .then(response => response.text()) // The server should return the URL of the converted .png image
            .then(pngImageUrl => {
                // Display the converted .png image in the preview section
                let span = document.createElement('span');
                span.innerHTML = ['<img id="preview" src="', pngImageUrl, '" title="', escape(f.name), '"/>'].join('');
                document.getElementById('imagePreviewContainer').innerHTML = '';
                document.getElementById('imagePreviewContainer').appendChild(span);
            })
            .catch(error => {
                console.error('Error converting .ppm image for preview:', error);
            });
    }
}




function submitImage() {
    let fileInput = document.getElementById('fileInput');
    if (fileInput.files.length === 0) {
        alert('Please select an image first.');
        return;
    }
    let gammaValue = document.getElementById('gammaInput').value;
    let coeffsValue = document.getElementById('coeffInput').value;
    let versionValue = document.getElementById('versionInput').value;
    let benchmarkValue = document.getElementById('benchmarkInput').value;

    let file = fileInput.files[0];
    let formData = new FormData();
    formData.append('image', file);
    formData.append('gamma', gammaValue);
    if (coeffsValue) {
        formData.append('coeffs', coeffsValue);
    }
    if (versionValue) {
        formData.append('version', versionValue);
    }
    if (benchmarkValue) {
        formData.append('benchmark', benchmarkValue);
    }

    fetch('/upload', {
        method: 'POST',
        body: formData
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            if (data.error) {
                alert(data.error);  // Handle errors from the server
            } else {
                let downloadLink = document.getElementById('downloadLink');
                downloadLink.href = data.processedImageUrl;
                document.getElementById('downloadSection').style.display = 'block';
            }
        })
        .catch(error => {
            console.error('Error uploading image:', error);
            alert('An error occurred while uploading the image. Please try again.');
        });
}
