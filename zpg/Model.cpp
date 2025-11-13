#include "Model.h"

Model::Model(const vector<float> &VERTICES): mVertices(VERTICES)
{

	glGenBuffers(1, &mVBO); // generate the VBO id
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVAO); //generate the VAO id
	glBindVertexArray(mVAO); //bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // normal

}

Model::Model(const vector<float>& VERTICES, int i) : mVertices(VERTICES)
{
    /*//vertex buffer object (VBO)
    GLuint VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);
    //vertex attribute object(VAO)
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));*/

    // Generate and bind VAO first
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Generate and bind VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);

    // Enable vertex attribute arrays
    glEnableVertexAttribArray(0); // position
    glEnableVertexAttribArray(1); // normal
    glEnableVertexAttribArray(2); // texcoord

    // Define vertex attribute layout (stride = 8 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

    // Unbind VAO for safety
    glBindVertexArray(0);
}

Model::Model() :mVAO(0), mVBO(0), mEBO(0) {}

Model::~Model()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	//glDeleteBuffers(1, &mEBO);
}

void Model::put() const
{
	glBindVertexArray(mVAO);
    if (!(mIndices.empty()))
        glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
    else
	    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) mVertices.size()/6);
}



bool Model::loadModelFromFile(const string& PATH)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(PATH,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenUVCoords);

    if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        std::cerr << "Failed to load model: " << PATH << "\n"
            << importer.GetErrorString() << std::endl;
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0]; 

    bool hasUVs = mesh->HasTextureCoords(0);
    vector<float> data;
    vector<unsigned int> indices;

	//detection of UVs
    int stride = hasUVs ? 8 : 6;
    data.reserve(mesh->mNumVertices * stride);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        aiVector3D pos = mesh->mVertices[i];
        data.push_back(pos.x);
        data.push_back(pos.y);
        data.push_back(pos.z);

        aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[i] : aiVector3D(0, 0, 0);
        data.push_back(normal.x);
        data.push_back(normal.y);
        data.push_back(normal.z);

        if (hasUVs)
        {
            aiVector3D uv = mesh->mTextureCoords[0][i];
            data.push_back(uv.x);
            data.push_back(uv.y);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    mVertices = move(data);
    mIndices = move(indices);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(3 * sizeof(float)));


    if (hasUVs)
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(6 * sizeof(float)));
    }

    glBindVertexArray(0);

   
    
    std::cout << "Model loaded via Assimp : " << PATH
        << " (" << mesh->mNumVertices << " vertices, UVs: "
        << (hasUVs ? "yes" : "no") << ")" << endl;




    return true;
}
