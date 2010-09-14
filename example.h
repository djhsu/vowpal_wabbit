#ifndef EX_H
#define EX_H

#include <stdint.h>
#include "v_array.h"

struct label_data {
  double label;
  float weight;
};

struct feature {
  float x;
  uint32_t weight_index;
  bool operator==(feature j){return weight_index == j.weight_index;}
};

struct audit_data {
  char* space;
  char* feature;
  size_t weight_index;
  float x;
  bool alloced;
};

struct example // core example datatype.
{
  void* ld;
  v_array<char> tag;//An identifier for the example.
  size_t example_counter;
  float time_stamp;

  bool sorted;//Are the features sorted or not?
  v_array<size_t> indices;
  float sum_feat_sq[256];
  v_array<feature> atomics[256]; // raw parsed data
  
  v_array<audit_data> audit_features[256];
  
  v_array<feature*> subsets[256];// helper for fast example expansion
  size_t num_features;//precomputed, cause it's fast&easy.
  float total_sum_feat_sq;//precomputed, cause it's kind of fast & easy.
  float partial_prediction;//shared data for prediction.
  float final_prediction;
  float loss;
  float eta_round;
  float global_weight;

  pthread_mutex_t lock; //thread coordination devices
  pthread_cond_t finished_sum;//barrier associated with lock.
  size_t threads_to_finish;
  bool in_use; //in use or not (for the parser)
  bool done; //set to false by setup_example()
};

struct partial_example { //used by the multisource parser
  int example_number;
  label_data ld;
  v_array<feature> features;
};

#endif
