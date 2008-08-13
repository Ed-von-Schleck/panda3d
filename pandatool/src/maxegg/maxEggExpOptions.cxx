/*
  maxEggExpOptions.cxx
  Created by Phillip Saltzman, 2/15/05
  Carnegie Mellon University, Entetainment Technology Center

  This file implements the classes that are used to choose
  what to export from 3D Studio max
*/


//Disable the forcing int to true or false performance warning
#pragma warning(disable: 4800)

void SetICustEdit(HWND wnd, int nIDDlgItem, char *text)
{ 
  ICustEdit *edit = GetICustEdit(GetDlgItem(wnd, nIDDlgItem));
  edit->SetText(text);
  ReleaseICustEdit(edit);
}

void SetICustEdit(HWND wnd, int nIDDlgItem, int n)
{
  char text[80];
  sprintf(text, "%d", n);
  ICustEdit *edit = GetICustEdit(GetDlgItem(wnd, nIDDlgItem));
  edit->SetText(text);
  ReleaseICustEdit(edit);
}

char *GetICustEditT(HWND wnd)
{
  static char buffer[2084];
  ICustEdit *edit = GetICustEdit(wnd);
  edit->GetText(buffer,2084);
  ReleaseICustEdit(edit);
  return buffer;
}

int GetICustEditI(HWND wnd, BOOL *valid)
{
  ICustEdit *edit = GetICustEdit(wnd);
  int i = edit->GetInt(valid);
  ReleaseICustEdit(edit);
  return i;
}

void ChunkSave(ISave *isave, int chunkid, int value)
{
  ULONG nb;
  isave->BeginChunk(chunkid);
  isave->Write(&value, sizeof(int), &nb);
  isave->EndChunk();
}

void ChunkSave(ISave *isave, int chunkid, ULONG value)
{
  ULONG nb;
  isave->BeginChunk(chunkid);
  isave->Write(&value, sizeof(ULONG), &nb);
  isave->EndChunk();
}

void ChunkSave(ISave *isave, int chunkid, bool value)
{
  ULONG nb;
  isave->BeginChunk(chunkid);
  isave->Write(&value, sizeof(bool), &nb);
  isave->EndChunk();
}

void ChunkSave(ISave *isave, int chunkid, char *value)
{
  ULONG nb;
  isave->BeginChunk(chunkid);
  int bytes = strlen(value) + 1;
  isave->Write(&bytes, sizeof(int), &nb);
  isave->Write(value, bytes, &nb);
  isave->EndChunk();
}

char *ChunkLoadString(ILoad *iload, char *buffer, int maxBytes)
{
  ULONG nb;
  int bytes;
  IOResult res;
  res = iload->Read(&bytes, sizeof(int), &nb);
  assert(res == IO_OK && bytes <= maxBytes);
  res = iload->Read(buffer, bytes, &nb);
  assert(res == IO_OK);
  return buffer;
}

int ChunkLoadInt(ILoad *iload)
{
  ULONG nb;
  int value;
  IOResult res;
  res = iload->Read(&value, sizeof(int), &nb);
  assert(res == IO_OK);
  return value;
}

int ChunkLoadULONG(ILoad *iload)
{
  ULONG nb, value;
  IOResult res;
  res = iload->Read(&value, sizeof(ULONG), &nb);
  assert(res == IO_OK);
  return value;
}

bool ChunkLoadBool(ILoad *iload)
{
  ULONG nb;
  bool value;
  IOResult res;
  res = iload->Read(&value, sizeof(bool), &nb);
  assert(res == IO_OK);
  return value;
}

void showAnimControls(HWND hWnd, BOOL val) {
  ShowWindow(GetDlgItem(hWnd, IDC_EF_LABEL), val);
  ShowWindow(GetDlgItem(hWnd, IDC_EF), val);
  ShowWindow(GetDlgItem(hWnd, IDC_SF_LABEL), val);
  SetWindowText(GetDlgItem(hWnd, IDC_EXP_SEL_FRAMES),
                val ? "Use Range:" : "Use Frame:");
}

void enableAnimControls(HWND hWnd, BOOL val) {
  EnableWindow(GetDlgItem(hWnd, IDC_SF_LABEL), val);
  EnableWindow(GetDlgItem(hWnd, IDC_SF), val);
  EnableWindow(GetDlgItem(hWnd, IDC_EF_LABEL), val);
  EnableWindow(GetDlgItem(hWnd, IDC_EF), val);
}

void enableChooserControls(HWND hWnd, BOOL val) {
  EnableWindow(GetDlgItem(hWnd, IDC_LIST_EXPORT), val);
  EnableWindow(GetDlgItem(hWnd, IDC_ADD_EXPORT), val);
  EnableWindow(GetDlgItem(hWnd, IDC_REMOVE_EXPORT), val);
}

#define ANIM_RAD_NONE 0
#define ANIM_RAD_EXPALL 1
#define ANIM_RAD_EXPSEL 2
#define ANIM_RAD_ALL    3
void enableAnimRadios(HWND hWnd, int mask) {
  EnableWindow(GetDlgItem(hWnd, IDC_EXP_ALL_FRAMES), mask & ANIM_RAD_EXPALL);
  EnableWindow(GetDlgItem(hWnd, IDC_EXP_SEL_FRAMES), mask & ANIM_RAD_EXPSEL);
}

// Handles the work of actually picking the target.
class AddNodeCB : public HitByNameDlgCallback
{
public:
  MaxEggExporter *ph; //Pointer to the parent class
  HWND hWnd;            //Handle to the parent dialog

  AddNodeCB (MaxEggExporter *instance, HWND wnd) : 
    ph(instance), hWnd(wnd) {}

  virtual TCHAR *dialogTitle() {return _T("Objects to Export");}
  virtual TCHAR *buttonText()  {return _T("Select");}
  virtual int filter(INode *node);
  virtual void proc(INodeTab &nodeTab);
};

//This tells what should be in the list
//Allow only triangular objects, nurbs, and joints
int AddNodeCB::filter(INode *node) {
  if (!node) return 0;
  
  Object *obj = node->EvalWorldState(0).obj;
  Control *c = node->GetTMController();
  NURBSSet getSet;
  bool is_bone = (node->GetBoneNodeOnOff() ||     //True for bones
     (c &&                                          //True for bipeds
     ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
     (c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
     (c->ClassID() == FOOTPRINT_CLASS_ID))));

  
  if (IsDlgButtonChecked(hWnd, IDC_ANIMATION) == BST_CHECKED)
    return is_bone && !ph->FindNode(node->GetHandle());
  else
    return (
    !is_bone &&
    ((obj->SuperClassID() == GEOMOBJECT_CLASS_ID && //Allow geometrics
      obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) ||
     (obj->SuperClassID() == SHAPE_CLASS_ID &&      //Allow CV NURBS
      obj->ClassID() == EDITABLE_SURF_CLASS_ID &&
      GetNURBSSet(obj, 0, getSet, TRUE) &&
      getSet.GetNURBSObject(0)->GetType() == kNCVCurve)) &&
    !ph->FindNode(node->GetHandle()));             //Only allow items not already selected
}

//Adds all of the selected items to the list
void AddNodeCB::proc(INodeTab &nodeTab) {
  for (int i = 0; i < nodeTab.Count(); i++)
    ph->AddNode(nodeTab[i]->GetHandle());
  ph->RefreshNodeList(hWnd);
}

//This callback class generates a list of nodes that have previously been selected
class RemoveNodeCB : public HitByNameDlgCallback
{
public:
    MaxEggExporter *ph; //Pointer to the parent class
    HWND hWnd;            //Handle to the parent dialog
    
    RemoveNodeCB (MaxEggExporter *instance, HWND wnd) : 
        ph(instance), hWnd(wnd) {}
    
    virtual TCHAR *dialogTitle() {return _T("Objects to Remove");}
    virtual TCHAR *buttonText()  {return _T("Remove");}
    virtual int filter(INode *node) {return (node && ph->FindNode(node->GetHandle()));}
    virtual void proc(INodeTab &nodeTab);
};


//Adds all of the selected items to the list
void RemoveNodeCB::proc(INodeTab &nodeTab) {
    for (int i = 0; i < nodeTab.Count(); i++)
        ph->RemoveNodeByHandle(nodeTab[i]->GetHandle());
    ph->RefreshNodeList(hWnd);
}

MaxEggExporter::MaxEggExporter ()
{
    _options._anim_convert = AC_model;
    _options._start_frame = INT_MIN;
    _options._end_frame = INT_MIN;
    _options._double_sided = false;
    _options._export_whole_scene = true;

    _checked = true;
    _choosing_nodes = false;
    _prev_type = AC_model;
    _filename = "";
}

BOOL CALLBACK MaxEggExporterProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
    char tempFilename[2048];
    //We pass in our plugin through the lParam variable. Let's convert it back.
    MaxEggExporter *imp = (MaxEggExporter*)GetWindowLongPtr(hWnd,GWLP_USERDATA); 
    if ( !imp && message != WM_INITDIALOG ) return FALSE;

    switch(message) {

    case WM_INITDIALOG:
        // this line is very necessary to pass the plugin as the lParam
        SetWindowLongPtr(hWnd,GWLP_USERDATA,lParam); 
        ((MaxEggExporter*)lParam)->UpdateUI(hWnd);
        return TRUE; break;
        
    case WM_CLOSE:
        EndDialog(hWnd, FALSE);
        return TRUE; break;
        
    case WM_COMMAND:
        //The modified control is found in the lower word of the wParam long.
        switch( LOWORD(wParam) ) {
        case IDC_MODEL:
            if (HIWORD(wParam) == BN_CLICKED) {
                SetWindowText(GetDlgItem(hWnd, IDC_EXPORT_SELECTED),
                              "Export Meshes:");
                enableAnimRadios(hWnd, ANIM_RAD_NONE);
                showAnimControls(hWnd, TRUE);
                enableAnimControls(hWnd, FALSE);
                if (imp->prev_type == MaxEggExporter::AT_chan) imp->ClearNodeList(hWnd);
                imp->prev_type = MaxEggExporter::AT_model;
                return TRUE;
            }
            break;
            
        case IDC_ANIMATION:
            if (HIWORD(wParam) == BN_CLICKED) {
                SetWindowText(GetDlgItem(hWnd, IDC_EXPORT_SELECTED),
                              "Export Bones:");
                enableAnimRadios(hWnd, ANIM_RAD_ALL);
                showAnimControls(hWnd, TRUE);
                enableAnimControls(hWnd, IsDlgButtonChecked(hWnd, IDC_EXP_SEL_FRAMES));
                if (imp->prev_type != MaxEggExporter::AT_chan) imp->ClearNodeList(hWnd);
                imp->prev_type = MaxEggExporter::AT_chan;
                return TRUE;
            }
            break;
        case IDC_BOTH:
            if (HIWORD(wParam) == BN_CLICKED) {
                SetWindowText(GetDlgItem(hWnd, IDC_EXPORT_SELECTED),
                              "Export Models:");
                enableAnimRadios(hWnd, ANIM_RAD_ALL);
                showAnimControls(hWnd, TRUE);
                enableAnimControls(hWnd, IsDlgButtonChecked(hWnd, IDC_EXP_SEL_FRAMES));
                if (imp->prev_type == MaxEggExporter::AT_chan) imp->ClearNodeList(hWnd);
                imp->prev_type = MaxEggExporter::AT_both;
                return TRUE;
            }
            break;
        case IDC_POSE:
            if (HIWORD(wParam) == BN_CLICKED) {
                SetWindowText(GetDlgItem(hWnd, IDC_EXPORT_SELECTED),
                              "Export Meshes:");
                enableAnimRadios(hWnd, ANIM_RAD_EXPSEL);
                showAnimControls(hWnd, FALSE);
                enableAnimControls(hWnd, TRUE);
                CheckRadioButton(hWnd, IDC_EXP_ALL_FRAMES, IDC_EXP_SEL_FRAMES, IDC_EXP_SEL_FRAMES);
                if (imp->prev_type == MaxEggExporter::AT_chan) imp->ClearNodeList(hWnd);
                imp->prev_type = MaxEggExporter::AT_pose;
                return TRUE;
            }
            break;
        case IDC_EXP_ALL_FRAMES:
            if (HIWORD(wParam) == BN_CLICKED) {
                enableAnimControls(hWnd, FALSE);
                return TRUE;
            }
            break;
            
        case IDC_EXP_SEL_FRAMES:
            if (HIWORD(wParam) == BN_CLICKED) {
                enableAnimControls(hWnd, TRUE);
                return TRUE;
            }
            break;

        case IDC_EXPORT_ALL:
            if (HIWORD(wParam) == BN_CLICKED) {
                enableChooserControls(hWnd, FALSE);
                return TRUE;
            }
            break;
            
        case IDC_EXPORT_SELECTED:
            if (HIWORD(wParam) == BN_CLICKED) {
                enableChooserControls(hWnd, TRUE);
                return TRUE;
            }
            break;

        case IDC_ADD_EXPORT:
            {
                if (!imp->choosingNodes) {
                    AddNodeCB PickCB(imp, hWnd);
                    imp->choosingNodes = true;
                    imp->maxInterface->DoHitByNameDialog(&PickCB);
                    imp->choosingNodes = false;
                }
            }
            return TRUE; break;

        case IDC_REMOVE_EXPORT:
            {
                if (!imp->choosingNodes) {
                    imp->choosingNodes = true;
                    RemoveNodeCB PickCB(imp, hWnd);
                    imp->maxInterface->DoHitByNameDialog(&PickCB);
                    imp->choosingNodes = false;
                }
            }
            return TRUE; break;

        case IDC_OK:
            if (imp->UpdateFromUI(hWnd)) EndDialog(hWnd, TRUE);
            return TRUE; break;
        case IDC_CANCEL:
            EndDialog(hWnd, FALSE);
            return TRUE; break;
        case IDC_BROWSE:
            OPENFILENAME ofn;
            strcpy(tempFilename, GetICustEditT(GetDlgItem(hWnd, IDC_FILENAME)));
            
            memset(&ofn, 0, sizeof(ofn));
            ofn.nMaxFile = 2047;
            ofn.lpstrFile = tempFilename;
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.Flags = OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST;
            ofn.lpstrDefExt = "egg";
            ofn.lpstrFilter = "Panda3D Egg Files (*.egg)\0*.egg\0All Files (*.*)\0*.*\0";
            
            SetFocus(GetDlgItem(hWnd, IDC_FILENAME));
            if (GetSaveFileName(&ofn))
                SetICustEdit(hWnd, IDC_FILENAME, ofn.lpstrFile);
            //else {
            //  char buf[255];
            //  sprintf(buf, "%d", CommDlgExtendedError());
            //  MessageBox(hWnd, buf, "Error on GetSaveFileName", MB_OK);
            //}
            return TRUE; break;
        case IDC_CHECK1:
            if (IsDlgButtonChecked(hWnd, IDC_CHECK1))
                if (MessageBox(hWnd, "Warning: Exporting double-sided polygons can severely hurt "
                               "performance in Panda3D.\n\nAre you sure you want to export them?",
                               "Panda3D Exporter", MB_YESNO | MB_ICONQUESTION) != IDYES)
                    CheckDlgButton(hWnd, IDC_CHECK1, BST_UNCHECKED);
            return TRUE; break;
        default:
            //char buf[255];
            //sprintf(buf, "%d", LOWORD(wParam));
            //MessageBox(hWnd, buf, "Unknown WParam", MB_OK);
            break;
        }
    }
    return FALSE;
}

void MaxEggExporter::SetAnimRange() {
    // Get the start and end frames and the animation frame rate from Max
    Interval anim_range = maxInterface->GetAnimRange();
    _min_frame = anim_range.Start()/GetTicksPerFrame();
    _max_frame = anim_range.End()/GetTicksPerFrame();
}

void MaxEggExporter::UpdateUI(HWND hWnd) {
    int typeButton = IDC_MODEL;
    int anim_exp = _options._start_frame == INT_MIN ? IDC_EXP_ALL_FRAMES : IDC_EXP_SEL_FRAMES;
    int model_exp = _options._export_whole_scene ? IDC_EXPORT_ALL : IDC_EXPORT_SELECTED;
    
    switch (anim_type) {
    case AT_chan: typeButton = IDC_ANIMATION; break;
    case AT_both: typeButton = IDC_BOTH; break;
    case AT_pose: typeButton = IDC_POSE; break;
    }
    
    _prev_convert = _options._anim_convert;
    
    CheckRadioButton(hWnd, IDC_MODEL, IDC_POSE, typeButton);
    SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(typeButton, BN_CLICKED), 0);
    CheckRadioButton(hWnd, IDC_EXPORT_ALL, IDC_EXPORT_SELECTED, model_exp);
    SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(model_exp, BN_CLICKED), 0);
    CheckRadioButton(hWnd, IDC_EXP_ALL_FRAMES, IDC_EXP_SEL_FRAMES, anim_exp);
    SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(anim_exp, BN_CLICKED), 0);
    
    CheckDlgButton(hWnd, IDC_CHECK1,
                   dblSided ? BST_CHECKED : BST_UNCHECKED);
    
    SetICustEdit(hWnd, IDC_FILENAME, filename);
    if (_options._start_frame != INT_MIN) {
        SetICustEdit(hWnd, IDC_SF, _options._start_frame);
        SetICustEdit(hWnd, IDC_EF, _options._end_frame);
    } else {
        SetICustEdit(hWnd, IDC_SF, _min_frame);
        SetICustEdit(hWnd, IDC_EF, _max_frame);
    }
    
    RefreshNodeList(hWnd);
}

void MaxEggExporter::ClearNodeList(HWND hWnd) {
    numNodes = 0;
    RefreshNodeList(hWnd);
}

void MaxEggExporter::RefreshNodeList(HWND hWnd) {
  //Clear and repopulate the node box
  HWND nodeLB = GetDlgItem(hWnd, IDC_LIST_EXPORT);
  SendMessage(nodeLB, LB_RESETCONTENT, 0, 0);
  for (int i = 0; i < numNodes; i++) {
      INode *temp = maxInterface->GetINodeByHandle(GetNode(i));
      TCHAR *name = _T("Unknown Node");
      if (temp) name = temp->GetName();
      SendMessage(nodeLB, LB_ADDSTRING, 0, (LPARAM)name);
  }
}

bool MaxEggExporter::UpdateFromUI(HWND hWnd) {
  BOOL valid;
  AnimationConvert newAnimType;
  int newSF = INT_MIN, newEF = INT_MIN;
  char msg[1024];

  if (IsDlgButtonChecked(hWnd, IDC_MODEL))          newAnimType = AC_model;
  else if (IsDlgButtonChecked(hWnd, IDC_ANIMATION)) newAnimType = AC_chan;
  else if (IsDlgButtonChecked(hWnd, IDC_BOTH))      newAnimType = AC_both;
  else                                              newAnimType = AC_pose;

  if (newAnimType != AT_model && IsDlgButtonChecked(hWnd, IDC_EXP_SEL_FRAMES)) {
      newSF = GetICustEditI(GetDlgItem(hWnd, IDC_SF), &valid);
      if (!valid) {
          MessageBox(hWnd, "Start Frame must be an integer", "Invalid Value", MB_OK | MB_ICONEXCLAMATION);
          return false;
      }
      if (newSF < minFrame) {
          sprintf(msg, "Start Frame must be at least %d", minFrame);
          MessageBox(hWnd, msg, "Invalid Value", MB_OK | MB_ICONEXCLAMATION);
          return false;
      }
      if (newSF > maxFrame) {
          sprintf(msg, "Start Frame must be at most %d", maxFrame);
          MessageBox(hWnd, msg, "Invalid Value", MB_OK | MB_ICONEXCLAMATION);
          return false;
      }
      if (newAnimType != AT_pose) {
          newEF = GetICustEditI(GetDlgItem(hWnd, IDC_EF), &valid);
          if (!valid) {
              MessageBox(hWnd, "End Frame must be an integer", "Invalid Value", MB_OK | MB_ICONEXCLAMATION);
              return false;
          }
          if (newEF > maxFrame) {
              sprintf(msg, "End Frame must be at most %d", maxFrame);
              MessageBox(hWnd, msg, "Invalid Value", MB_OK | MB_ICONEXCLAMATION);
              return false;
          }
          if (newEF < newSF) {
              MessageBox(hWnd, "End Frame must be greater than the start frame", "Invalid Value", MB_OK | MB_ICONEXCLAMATION);
              return false;
          }
      }
  }

  char *temp = GetICustEditT(GetDlgItem(hWnd, IDC_FILENAME));
  if (!strlen(temp)) {
      MessageBox(hWnd, "The filename cannot be empty", "Invalid Value", MB_OK | MB_ICONEXCLAMATION);
      return false;
  }
  _options._file_name = temp;
  if ((options._file_name.size() < 4) || (_options._file_name.compare(options._file_name.size()-4, 4, ".egg"))) {
      _options._file_name += ".egg";
  }
  
  int slash = _options._file_name.rfind('\\');
  if (slash == _options._file_name.npos) {
      slash = 0;
  } else {
      slash += 1;
  }

  _options._short_name = _options._file_name.substr(slash, _options._file_name.size() - 4 - slash);

  _options._start_frame = newSF;
  _options._end_frame = newEF;
  _options._anim_convert = newAnimType;
  _options._double_sided = IsDlgButtonChecked(hWnd, IDC_CHECK1);
  _options._export_whole_scene = IsDlgButtonChecked(hWnd, IDC_EXPORT_ALL);
  return true;
}

bool MaxEggExporter::FindNode(ULONG INodeHandle) {
    for (int i = 0; i < _options._node_list.size(); i++) 
        if (_options._node_list[i] == INodeHandle) return true;
    return false;
}

void MaxEggExporter::AddNode(ULONG INodeHandle) {
  if (FindNode(INodeHandle)) return; 
  _options._node_list.push_back(INodeHandle);
}

void MaxEggExporter::CullBadNodes() {
  if (!_options._max_interface) return;
  std::vector<ULONG> good;
  for (int i=0; i<_options._node_list.size(); i++) {
      ULONG handle = _options._node_list[i];
      if (maxInterface->GetINodeByHandle(handle)) {
          good.push_back(handle);
      }
  }
  _options._node_list = good;
}

void MaxEggExporter::RemoveNode(int i) {
    if (i >= 0 && i < _options._node_list.size()) {
        for (int j = i+1; j < _options._node_list.size())
            _options._node_list[i++] = _options._node_list[j++];
        _options._node_list.pop_back();
    }
}

void MaxEggExporter::RemoveNodeByHandle(ULONG INodeHandle) {
    for (int i = 0; i < _options._node_list.size(); i++) {
        if (_options._node_list[i] == INodeHandle) {
            RemoveNode(i);
            return;
        }
    }
}

IOResult MaxEggExporter::Save(ISave *isave) {
    isave->BeginChunk(CHUNK_EGG_EXP_OPTIONS);
    ChunkSave(isave, CHUNK_ANIM_TYPE, _options._anim_convert);
    ChunkSave(isave, CHUNK_FILENAME, _options._file_name.c_str());
    ChunkSave(isave, CHUNK_SHORTNAME, _options._short_name.c_str());
    ChunkSave(isave, CHUNK_SF, _options._start_frame);
    ChunkSave(isave, CHUNK_EF, _options._end_frame);
    ChunkSave(isave, CHUNK_DBL_SIDED, _options._double_sided);
    ChunkSave(isave, CHUNK_EGG_CHECKED, _checked);
    ChunkSave(isave, CHUNK_EXPORT_FULL, _options._export_whole_scene);
    isave->BeginChunk(CHUNK_NODE_LIST);
    for (int i = 0; i < _options._node_list.size(); i++)
        ChunkSave(isave, CHUNK_NODE_HANDLE, _options._node_list[i]);
    isave->EndChunk();
    isave->EndChunk();
    return IO_OK;
} 

IOResult MaxEggExporter::Load(ILoad *iload) {
    IOResult res = iload->OpenChunk();
    
    while (res == IO_OK) {
        switch(iload->CurChunkID()) {
        case CHUNK_ANIM_TYPE: _options._anim_convert = (AnimationConvert)ChunkLoadInt(iload); break;
        case CHUNK_FILENAME: ChunkLoadString(iload, filename, sizeof(filename)); break;
        case CHUNK_SHORTNAME: ChunkLoadString(iload, shortName, sizeof(shortName)); break;
        case CHUNK_SF: _options._start_frame = ChunkLoadInt(iload); break;
        case CHUNK_EF: _options._end_frame = ChunkLoadInt(iload); break;
        case CHUNK_DBL_SIDED: _options._double_sided = ChunkLoadBool(iload); break;
        case CHUNK_EGG_CHECKED: _checked = ChunkLoadBool(iload); break;
        case CHUNK_EXPORT_FULL: _options._export_whole_scene = ChunkLoadBool(iload); break;
        case CHUNK_NODE_LIST:
            res = iload->OpenChunk();
            while (res == IO_OK) {
                if (iload->CurChunkID() == CHUNK_NODE_HANDLE) AddNode(ChunkLoadULONG(iload));
                iload->CloseChunk();
                res = iload->OpenChunk();
            }
            break;
        }
        iload->CloseChunk();
        res = iload->OpenChunk();
    }
    
    if (res == IO_END) return IO_OK;
    return IO_ERROR;
}

bool MaxEggExporter::DoExport(IObjParam *ip, bool autoOverwrite, bool saveLog) 
{
    MaxToEggConverter converter;
    
    // Now, we fill out the necessary fields of the converter, which does all
    // the necessary work.
    converter.setMaxInterface( (Interface*)ip );
    // converter.set_path_replace( new PathReplace() );
    // converter.set_character_name("max character");
    // converter.set_start_frame(_options._start_frame);
    // converter.set_end_frame(_options._end_frame);
    // converter.set_frame_inc(_frame_inc);
    // converter.set_neutral_frame(_neutral_frame);
    // converter.set_input_frame_rate(_input_frame_rate);
    // converter.set_output_frame_rate(_output_frame_rate);
    // converter.set_double_sided(false);
    
    switch (anim_type) {
    case AT_chan:  converter.set_animation_convert(AC_chan); break;
    case AT_pose:  converter.set_animation_convert(AC_pose); break;
    case AT_both:  converter.set_animation_convert(AC_both); break;
    case AT_model: converter.set_animation_convert(AC_model); break;
    default:       converter.set_animation_convert(AC_model); break;
    }
    
    if (expWholeScene) converter.set_selection_list(NULL, 0);
    else converter.set_selection_list(nodeList, numNodes);
    
    // Now, do the actual file conversion.
    PT(EggData) data = converter.convert();
    if (data != (EggData*)NULL) {
        // write_egg_file();
        Logger::Log( MTEC, Logger::SAT_DEBUG_SPAM_LEVEL, "Egg file written!" );
    } else {
        Logger::Log( MTEC, Logger::SAT_DEBUG_SPAM_LEVEL, "Conversion failed!" );
    }
    return true;
}
