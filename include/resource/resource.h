#ifndef QGE_RESOURCE_H
#define QGE_RESOURCE_H

#include <string>

namespace quasi_game_engine {
   class RenderManager;
   class Logger;

   // Template for data in function. Automatically has get and set built in
   template <typename T> class data {
   public:
      data() {};
      data(T new_value) : value(new_value) {};
      inline void set(T new_value) { value = new_value; };
      inline T get(void) { return value; };

   private:
      T value;
   };


   struct Asset {
      Asset(std::string file_location_in, std::string asset_name_in = NULL, uint guid_in = 0)
          : file_location(file_location_in), asset_name(asset_name_in), guid(guid_in) {}

      std::string file_location, asset_name;
      uint guid;
   };




   class Resource {
   /*Note-----------------------------------------------------------------------
   - Wanted to load files in constructors but we don't know the size before file is loaded
   ---------------------------------------------------------------------------*/
   public:
      virtual ~Resource();

      virtual uint GetResource() = 0;
      //getNumParents
      //getChildren

      virtual void LoadToGraphics(RenderManager* render_manager);

      //addchild
      //add parent?

      static void SetLogger(Logger* logger) { logger_ = logger; };

   protected:
      Resource(Asset asset);

      Asset asset_;
      static Logger* logger_;

      //numParents
      //vector of children pointers
      //GUID
   };


   class Model {
      Model();
      ~Model();
   };


   //All of these have been assigned public, need to find out if this is correct
   class Mesh : public Resource {
   public:
      Mesh(float* verts, float* norms,
           float* uvs, int* faces,
            int num_verts, int num_faces,
            Asset asset);
      ~Mesh();

      inline uint GetResource() { return vao_; };

      void GetMesh(float** verts, float** norms,
                   float** uvs, int** faces,
                   int &num_verts, int &num_faces);

      void LoadToGraphics(RenderManager* render_manager);

      inline void SetVAO(int vao) { vao_ = vao; };

   private:
      float *verts_, *norms_, *uvs_;
      int *faces_, num_verts_, num_faces_;
      uint vao_;
   };

   // class Skeleton : public Resource {
   // public:
   //    Skeleton();
   //    ~Skeleton();
   //
   //    unsigned int getResource() { return 0; };
   //
   // private:
   //    //resources
   //    //graphics ids
   // };


   // class Animation : public Resource {
   // public:
   //    Animation();
   //    ~Animation();
   //
   //    unsigned int getResource() { return 0; };
   //
   // private:
   //    //resources
   //    //graphics ids
   // };

   // class Material : public Resource {
   // public:
   //    Material();
   //    ~Material();
   //
   //    unsigned int getResource() { return 0; };
   //
   // private:
   //    //resources
   //    //graphics ids
   // };




   // class Font : public Resource {
   // public:
   //    Font();
   //    ~Font();
   //
   //    unsigned int getResource() { return 0; };
   //
   // private:
   //    //resources
   //    //graphics ids
   // };

} // namespace quasi_game_engine





#endif // QGE_RESOURCE_H
