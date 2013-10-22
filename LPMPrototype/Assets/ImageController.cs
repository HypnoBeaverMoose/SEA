using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ImageController : MonoBehaviour 
{
    public bool isZoomed { get { return Camera.mainCamera.orthographicSize < m_initialOrthoSize; } }
    private float m_initialOrthoSize;

    void Start ()    
    {
        float linear_size = Camera.mainCamera.orthographicSize * 0.2f;
        Vector3 size = new Vector3(linear_size * Camera.mainCamera.aspect, 1.0f, linear_size);
        transform.localScale = size;
        m_initialOrthoSize = Camera.mainCamera.orthographicSize;
    }

    void Update()
    {
    }

    public void SetImage(Texture2D texture)
    {
        renderer.material.mainTexture = texture;
    }

    public void Zoom(bool zoomIn, Vector2 pos)
    {
        if (zoomIn)
        {
            transform.Translate(-pos.x, 0, -pos.y);
            Camera.mainCamera.orthographicSize = 1;
        }
        else
        {
            transform.localPosition = Vector3.zero;
            Camera.mainCamera.orthographicSize = m_initialOrthoSize;
        }

    }
}
