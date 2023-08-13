using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowCam : MonoBehaviour
{
    public Transform Target;
    Vector3 StartOffset;
    // Start is called before the first frame update
    void Start() {
        StartOffset = this.transform.position - Target.position;
    }

    // Update is called once per frame
    void Update() {
       this.transform.position = Vector3.MoveTowards(this.transform.position, Target.position+StartOffset, 1); 
    }
}
