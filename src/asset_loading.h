#pragma once
#include <vector>

struct Model 
{
    std::vector <glm::vec3> vertices;
    std::vector <glm::vec2> uvs;
    std::vector <glm::vec3> normals;
};


bool LoadOBJ (const char* filepath, Model& model) 
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;
    
    FILE* file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Failed to open file.\n");
        return false;
    }
    
    while (1)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;
        
        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            tempVertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            tempUVs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]
                                 );
            if (matches != 9)
            {
                printf("File can't be read with simple parser");
                return false;
            }
            
            for (int i = 0; i < 3; i++)
            {
                vertexIndices.push_back(vertexIndex[i]);
                uvIndices.push_back(uvIndex[i]);
                normalIndices.push_back(normalIndex[i]);
            }
        }
        else 
        {
            char commentBuffer[1000];
            fgets(commentBuffer, 1000, file);
        }
    }
    
    
    for (int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = tempVertices[vertexIndex - 1];
        model.vertices.push_back(vertex);
        
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = tempUVs[uvIndex - 1];
        model.uvs.push_back(uv);
        
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = tempVertices[normalIndex - 1];
        model.normals.push_back(normal);
    }
    
    fclose(file);
    
    return true;
}

GLuint LoadBMP (const char* imagePath)
{
    unsigned char  header[54];    // BMP files have a 54 byte header
    unsigned int   dataPos;       // Position in file where actual data begins
    unsigned int   width, height; 
    unsigned int   imageSize;     // width * height * 3 (3 : 1 byte each for R, G, and B) 
    unsigned char* data;          // Actual RGB data
    
    // Open the file
    FILE* file = fopen(imagePath, "rb");
    if (!file) 
    {
        printf("Image could not be opened.\n");
        return 0;
    }
    
    // If not 54 bytes read, we have problem
    if (fread(header, 1, 54, file) != 54) 
    {
        printf("Not a correct BMP file.\n");
        return 0;
    }
    
    // First two characters of file must be BM
    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Not a correct BMP file.\n");
        return 0;
    }
    
    dataPos   = *(int*) &(header[0x0A]);
    imageSize = *(int*) &(header[0x22]);
    width     = *(int*) &(header[0x12]);
    height    = *(int*) &(header[0x16]);
    
    // If BMP file is misformatted, infer the data
    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0)   dataPos   = 54; // End of header
    
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);
    
    // Everything is in memory now, the file can be closed
    fclose(file);
    
    // Create OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // Bind the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    // When magnifying image, use linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // When minifying the image, use a linear blend of two mipmaps, each filtered linearly
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return textureID;
}

// TODO(Bryson): Implement compressed file loading
GLuint LoadDDS(const char* imagePath);