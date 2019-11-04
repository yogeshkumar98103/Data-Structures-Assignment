//
// Created by Yogesh Kumar on 2019-04-17.
//

#ifndef CLION_WORKSPACE_SORT_H
#define CLION_WORKSPACE_SORT_H

void bubbleSort(int arr[], int size){
    int temp;
    for(int i = 0; i<size-1; i++){
        for(int j = 0; j< size-i-1; j++){
            if(arr[j] > arr[j+1]){
                // Reverse the inequality to sort in descending order
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

#endif //CLION_WORKSPACE_SORT_H
