<<<<<<< HEAD
// Copyright (c) 2009-2010 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

DECLARE_EVENT_TYPE(wxEVT_UITHREADCALL, -1)



extern wxLocale g_locale;



void HandleCtrlA(wxKeyEvent& event);
string FormatTxStatus(const CWalletTx& wtx);
void UIThreadCall(boost::function0<void>);
int ThreadSafeMessageBox(const string& message, const string& caption="Message", int style=wxOK, wxWindow* parent=NULL, int x=-1, int y=-1);
bool ThreadSafeAskFee(int64 nFeeRequired, const string& strCaption, wxWindow* parent);
void CalledSetStatusBar(const string& strText, int nField);
void MainFrameRepaint();
void CreateMainWindow();
void SetStartOnSystemStartup(bool fAutoStart);




inline int MyMessageBox(const wxString& message, const wxString& caption="Message", int style=wxOK, wxWindow* parent=NULL, int x=-1, int y=-1)
{
#ifdef GUI
    if (!fDaemon)
        return wxMessageBox(message, caption, style, parent, x, y);
#endif
    printf("wxMessageBox %s: %s\n", string(caption).c_str(), string(message).c_str());
    fprintf(stderr, "%s: %s\n", string(caption).c_str(), string(message).c_str());
    return wxOK;
}
#define wxMessageBox  MyMessageBox






class CMainFrame : public CMainFrameBase
{
protected:
    // Event handlers
    void OnNotebookPageChanged(wxNotebookEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnIconize(wxIconizeEvent& event);
    void OnMouseEvents(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event) { HandleCtrlA(event); }
    void OnIdle(wxIdleEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnPaintListCtrl(wxPaintEvent& event);
    void OnMenuFileExit(wxCommandEvent& event);
    void OnMenuOptionsGenerate(wxCommandEvent& event);
    void OnUpdateUIOptionsGenerate(wxUpdateUIEvent& event);
    void OnMenuOptionsChangeYourAddress(wxCommandEvent& event);
    void OnMenuOptionsOptions(wxCommandEvent& event);
    void OnMenuHelpAbout(wxCommandEvent& event);
    void OnButtonSend(wxCommandEvent& event);
    void OnButtonAddressBook(wxCommandEvent& event);
    void OnSetFocusAddress(wxFocusEvent& event);
    void OnMouseEventsAddress(wxMouseEvent& event);
    void OnButtonNew(wxCommandEvent& event);
    void OnButtonCopy(wxCommandEvent& event);
    void OnListColBeginDrag(wxListEvent& event);
    void OnListItemActivated(wxListEvent& event);
    void OnListItemActivatedProductsSent(wxListEvent& event);
    void OnListItemActivatedOrdersSent(wxListEvent& event);
    void OnListItemActivatedOrdersReceived(wxListEvent& event);
	
public:
    /** Constructor */
    CMainFrame(wxWindow* parent);
    ~CMainFrame();

    // Custom
    enum
    {
        ALL = 0,
        SENTRECEIVED = 1,
        SENT = 2,
        RECEIVED = 3,
    };
    int nPage;
    wxListCtrl* m_listCtrl;
    bool fShowGenerated;
    bool fShowSent;
    bool fShowReceived;
    bool fRefreshListCtrl;
    bool fRefreshListCtrlRunning;
    bool fOnSetFocusAddress;
    unsigned int nListViewUpdated;
    bool fRefresh;

    void OnUIThreadCall(wxCommandEvent& event);
    int GetSortIndex(const string& strSort);
    void InsertLine(bool fNew, int nIndex, uint256 hashKey, string strSort, const wxString& str1, const wxString& str2, const wxString& str3, const wxString& str4, const wxString& str5);
    bool DeleteLine(uint256 hashKey);
    bool InsertTransaction(const CWalletTx& wtx, bool fNew, int nIndex=-1);
    void RefreshListCtrl();
    void RefreshStatusColumn();
};




class CTxDetailsDialog : public CTxDetailsDialogBase
{
protected:
    // Event handlers
    void OnButtonOK(wxCommandEvent& event);

public:
    /** Constructor */
    CTxDetailsDialog(wxWindow* parent, CWalletTx wtx);

    // State
    CWalletTx wtx;
};



class COptionsDialog : public COptionsDialogBase
{
protected:
    // Event handlers
    void OnListBox(wxCommandEvent& event);
    void OnKillFocusTransactionFee(wxFocusEvent& event);
    void OnCheckBoxLimitProcessors(wxCommandEvent& event);
    void OnCheckBoxUseProxy(wxCommandEvent& event);
    void OnKillFocusProxy(wxFocusEvent& event);

    void OnButtonOK(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
    void OnButtonApply(wxCommandEvent& event);

public:
    /** Constructor */
    COptionsDialog(wxWindow* parent);

    // Custom
    bool fTmpStartOnSystemStartup;
    bool fTmpMinimizeOnClose;
    void SelectPage(int nPage);
    CAddress GetProxyAddr();
};



class CAboutDialog : public CAboutDialogBase
{
protected:
    // Event handlers
    void OnButtonOK(wxCommandEvent& event);

public:
    /** Constructor */
    CAboutDialog(wxWindow* parent);
};



class CSendDialog : public CSendDialogBase
{
protected:
    // Event handlers
    void OnKeyDown(wxKeyEvent& event) { HandleCtrlA(event); }
    void OnTextAddress(wxCommandEvent& event);
    void OnKillFocusAmount(wxFocusEvent& event);
    void OnButtonAddressBook(wxCommandEvent& event);
    void OnButtonPaste(wxCommandEvent& event);
    void OnButtonSend(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
	
public:
    /** Constructor */
    CSendDialog(wxWindow* parent, const wxString& strAddress="");

    // Custom
    bool fEnabledPrev;
    string strFromSave;
    string strMessageSave;
};



class CSendingDialog : public CSendingDialogBase
{
public:
    // Event handlers
    void OnClose(wxCloseEvent& event);
    void OnButtonOK(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
	
public:
    /** Constructor */
    CSendingDialog(wxWindow* parent, const CAddress& addrIn, int64 nPriceIn, const CWalletTx& wtxIn);
    ~CSendingDialog();

    // State
    CAddress addr;
    int64 nPrice;
    CWalletTx wtx;
    wxDateTime start;
    char pszStatus[10000];
    bool fCanCancel;
    bool fAbort;
    bool fSuccess;
    bool fUIDone;
    bool fWorkDone;

    void Close();
    void Repaint();
    bool Status();
    bool Status(const string& str);
    bool Error(const string& str);
    void StartTransfer();
    void OnReply2(CDataStream& vRecv);
    void OnReply3(CDataStream& vRecv);
};

void SendingDialogStartTransfer(void* parg);
void SendingDialogOnReply2(void* parg, CDataStream& vRecv);
void SendingDialogOnReply3(void* parg, CDataStream& vRecv);



class CAddressBookDialog : public CAddressBookDialogBase
{
protected:
    // Event handlers
    void OnNotebookPageChanged(wxNotebookEvent& event);
    void OnListEndLabelEdit(wxListEvent& event);
    void OnListItemSelected(wxListEvent& event);
    void OnListItemActivated(wxListEvent& event);
    void OnButtonDelete(wxCommandEvent& event);
    void OnButtonCopy(wxCommandEvent& event);
    void OnButtonEdit(wxCommandEvent& event);
    void OnButtonNew(wxCommandEvent& event);
    void OnButtonOK(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

public:
    /** Constructor */
    CAddressBookDialog(wxWindow* parent, const wxString& strInitSelected, int nPageIn, bool fDuringSendIn);

    // Custom
    enum
    {
        SENDING = 0,
        RECEIVING = 1,
    };
    int nPage;
    wxListCtrl* m_listCtrl;
    bool fDuringSend;
    wxString GetAddress();
    wxString GetSelectedAddress();
    wxString GetSelectedSendingAddress();
    wxString GetSelectedReceivingAddress();
    bool CheckIfMine(const string& strAddress, const string& strTitle);
};



class CGetTextFromUserDialog : public CGetTextFromUserDialogBase
{
protected:
    // Event handlers
    void OnButtonOK(wxCommandEvent& event)     { EndModal(true); }
    void OnButtonCancel(wxCommandEvent& event) { EndModal(false); }
    void OnClose(wxCloseEvent& event)          { EndModal(false); }

    void OnKeyDown(wxKeyEvent& event)
    {
        if (event.GetKeyCode() == '\r' || event.GetKeyCode() == WXK_NUMPAD_ENTER)
            EndModal(true);
        else
            HandleCtrlA(event);
    }

public:
    /** Constructor */
    CGetTextFromUserDialog(wxWindow* parent,
                           const string& strCaption,
                           const string& strMessage1,
                           const string& strValue1="",
                           const string& strMessage2="",
                           const string& strValue2="") : CGetTextFromUserDialogBase(parent, wxID_ANY, strCaption)
    {
        int x = GetSize().GetWidth();
        int y = GetSize().GetHeight();
        m_staticTextMessage1->SetLabel(strMessage1);
        m_textCtrl1->SetValue(strValue1);
        y += wxString(strMessage1).Freq('\n') * 14;
        if (!strMessage2.empty())
        {
            m_staticTextMessage2->Show(true);
            m_staticTextMessage2->SetLabel(strMessage2);
            m_textCtrl2->Show(true);
            m_textCtrl2->SetValue(strValue2);
            y += 46 + wxString(strMessage2).Freq('\n') * 14;
        }
#ifndef __WXMSW__
        x *= 1.14;
        y *= 1.14;
#endif
        SetSize(x, y);
    }

    // Custom
    string GetValue()  { return (string)m_textCtrl1->GetValue(); }
    string GetValue1() { return (string)m_textCtrl1->GetValue(); }
    string GetValue2() { return (string)m_textCtrl2->GetValue(); }
};



class CMyTaskBarIcon : public wxTaskBarIcon
{
protected:
    // Event handlers
    void OnLeftButtonDClick(wxTaskBarIconEvent& event);
    void OnMenuRestore(wxCommandEvent& event);
    void OnMenuOptions(wxCommandEvent& event);
    void OnUpdateUIGenerate(wxUpdateUIEvent& event);
    void OnMenuGenerate(wxCommandEvent& event);
    void OnMenuExit(wxCommandEvent& event);

public:
    CMyTaskBarIcon() : wxTaskBarIcon()
    {
        Show(true);
    }

    void Show(bool fShow=true);
    void Hide();
    void Restore();
    void UpdateTooltip();
    virtual wxMenu* CreatePopupMenu();

DECLARE_EVENT_TABLE()
};
=======
// Copyright (c) 2009 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.




DECLARE_EVENT_TYPE(wxEVT_CROSSTHREADCALL, -1)
DECLARE_EVENT_TYPE(wxEVT_REPLY1, -1)
DECLARE_EVENT_TYPE(wxEVT_REPLY2, -1)
DECLARE_EVENT_TYPE(wxEVT_REPLY3, -1)
DECLARE_EVENT_TYPE(wxEVT_TABLEADDED, -1)
DECLARE_EVENT_TYPE(wxEVT_TABLEUPDATED, -1)
DECLARE_EVENT_TYPE(wxEVT_TABLEDELETED, -1)

enum
{
    UICALL_ADDORDER = 1,
    UICALL_UPDATEORDER,
};



extern void HandleCtrlA(wxKeyEvent& event);
extern string DateTimeStr(int64 nTime);
extern string FormatTxStatus(const CWalletTx& wtx);
extern void CrossThreadCall(int nID, void* pdata);
extern void MainFrameRepaint();
extern void Shutdown(void* parg);
void ApplyUISettings();

// UI settings
extern int minimizeToTray;
extern int closeToTray;
extern int startOnSysBoot;
extern int askBeforeClosing;
extern int alwaysShowTrayIcon;



class CMainFrame : public CMainFrameBase
{
protected:
    // Event handlers
    void OnClose(wxCloseEvent& event);
    void OnIconize( wxIconizeEvent& event );
    void OnMouseEvents(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event) { HandleCtrlA(event); }
    void OnIdle(wxIdleEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnPaintListCtrl(wxPaintEvent& event);
    void OnMenuFileExit(wxCommandEvent& event);
    void OnMenuOptionsGenerate(wxCommandEvent& event);
    void OnMenuOptionsChangeYourAddress(wxCommandEvent& event);
    void OnMenuOptionsOptions(wxCommandEvent& event);
    void OnMenuHelpAbout(wxCommandEvent& event);
    void OnButtonSend(wxCommandEvent& event);
    void OnButtonAddressBook(wxCommandEvent& event);
    void OnSetFocusAddress(wxFocusEvent& event);
    void OnMouseEventsAddress(wxMouseEvent& event);
    void OnButtonCopy(wxCommandEvent& event);
    void OnButtonChange(wxCommandEvent& event);
    void OnListColBeginDrag(wxListEvent& event);
    void OnListItemActivatedAllTransactions(wxListEvent& event);
    void OnListItemActivatedProductsSent(wxListEvent& event);
    void OnListItemActivatedOrdersSent(wxListEvent& event);
    void OnListItemActivatedOrdersReceived(wxListEvent& event);
    void OnUpdateMenuGenerate( wxUpdateUIEvent& event );
	
public:
    /** Constructor */
    CMainFrame(wxWindow* parent);
    ~CMainFrame();

    // Custom
    bool fRefreshListCtrl;
    bool fRefreshListCtrlRunning;
    bool fOnSetFocusAddress;
    CBlockIndex* pindexBestLast;
    set<uint256> setUnmaturedDisplayed;

    void OnCrossThreadCall(wxCommandEvent& event);
    void InsertLine(bool fNew, int nIndex, uint256 hashKey, string strSort, const wxString& str1, const wxString& str2, const wxString& str3, const wxString& str4, const wxString& str5);
    void InsertTransaction(const CWalletTx& wtx, bool fNew, int nIndex=-1);
    void RefreshListCtrl();
    void RefreshStatus();
    void SendToTray();
};




class CTxDetailsDialog : public CTxDetailsDialogBase
{
protected:
    // Event handlers
    void OnButtonOK(wxCommandEvent& event);

public:
    /** Constructor */
    CTxDetailsDialog(wxWindow* parent, CWalletTx wtx);

    // State
    CWalletTx wtx;
};



class COptionsPanelBitcoin : public COptionsPanelBitcoinBase
{
protected:
    // Event handlers
	void OnKillFocusTransactionFee( wxFocusEvent& event );

public:
    /** Constructor */
    COptionsPanelBitcoin(wxWindow* parent);
};



class COptionsPanelUI : public COptionsPanelUIBase
{
protected:
    // Event handlers
	void OnOptionsChanged( wxCommandEvent& event );

public:
    /** Constructor */
    COptionsPanelUI(wxWindow* parent);
};



class COptionsDialog : public COptionsDialogBase
{
protected:
    // Event handlers
	void MenuSelChanged( wxTreeEvent& event );
    void OnButtonOK(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);

    // Panels
    COptionsPanelBitcoin* panelBitcoin;
    COptionsPanelUI* panelUI;
    wxPanel* currentPanel;
public:
    /** Constructor */
    COptionsDialog(wxWindow* parent);
};



class CAboutDialog : public CAboutDialogBase
{
protected:
    // Event handlers
    void OnButtonOK(wxCommandEvent& event);

public:
    /** Constructor */
    CAboutDialog(wxWindow* parent);
};



class CSendDialog : public CSendDialogBase
{
protected:
    // Event handlers
    void OnKeyDown(wxKeyEvent& event) { HandleCtrlA(event); }
    void OnTextAddress(wxCommandEvent& event);
    void OnKillFocusAmount(wxFocusEvent& event);
    void OnButtonAddressBook(wxCommandEvent& event);
    void OnButtonPaste(wxCommandEvent& event);
    void OnButtonSend(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
	
public:
    /** Constructor */
    CSendDialog(wxWindow* parent, const wxString& strAddress="");
};



class CSendingDialog : public CSendingDialogBase
{
public:
    // Event handlers
    void OnClose(wxCloseEvent& event);
    void OnButtonOK(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
	
public:
    /** Constructor */
    CSendingDialog(wxWindow* parent, const CAddress& addrIn, int64 nPriceIn, const CWalletTx& wtxIn);
    ~CSendingDialog();

    // State
    CAddress addr;
    int64 nPrice;
    CWalletTx wtx;
    wxDateTime start;
    string strStatus;
    bool fCanCancel;
    bool fAbort;
    bool fSuccess;
    bool fUIDone;
    bool fWorkDone;

    void Close();
    void Repaint();
    bool Status();
    bool Status(const string& str);
    bool Error(const string& str);
    void StartTransfer();
    void OnReply2(CDataStream& vRecv);
    void OnReply3(CDataStream& vRecv);
};

void SendingDialogStartTransfer(void* parg);
void SendingDialogOnReply2(void* parg, CDataStream& vRecv);
void SendingDialogOnReply3(void* parg, CDataStream& vRecv);



class CYourAddressDialog : public CYourAddressDialogBase
{
protected:
    // Event handlers
    void OnListEndLabelEdit(wxListEvent& event);
    void OnListItemSelected(wxListEvent& event);
    void OnListItemActivated(wxListEvent& event);
    void OnButtonRename(wxCommandEvent& event);
    void OnButtonNew(wxCommandEvent& event);
    void OnButtonCopy(wxCommandEvent& event);
    void OnButtonOK(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

public:
    /** Constructor */
    CYourAddressDialog(wxWindow* parent);
    CYourAddressDialog(wxWindow* parent, const string& strInitSelected);

    // Custom
    wxString GetAddress();
};



class CAddressBookDialog : public CAddressBookDialogBase
{
protected:
    // Event handlers
    void OnListEndLabelEdit(wxListEvent& event);
    void OnListItemSelected(wxListEvent& event);
    void OnListItemActivated(wxListEvent& event);
    void OnButtonEdit(wxCommandEvent& event);
    void OnButtonDelete(wxCommandEvent& event);
    void OnButtonNew(wxCommandEvent& event);
    void OnButtonCopy(wxCommandEvent& event);
    void OnButtonOK(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

public:
    /** Constructor */
    CAddressBookDialog(wxWindow* parent, const wxString& strInitSelected, bool fSendingIn);

    // Custom
    bool fSending;
    wxString GetAddress();
    bool CheckIfMine(const string& strAddress, const string& strTitle);
};



class CProductsDialog : public CProductsDialogBase
{
protected:
    // Event handlers
    void OnKeyDown(wxKeyEvent& event) { HandleCtrlA(event); }
    void OnCombobox(wxCommandEvent& event);
    void OnButtonSearch(wxCommandEvent& event);
    void OnListItemActivated(wxListEvent& event);

public:
    /** Constructor */
    CProductsDialog(wxWindow* parent);

    // Custom
    vector<CProduct> m_vProduct;
};



class CEditProductDialog : public CEditProductDialogBase
{
protected:
    // Event handlers
    void OnKeyDown(wxKeyEvent& event) { HandleCtrlA(event); }
    void OnButtonDel0(wxCommandEvent& event);
    void OnButtonDel1(wxCommandEvent& event);
    void OnButtonDel2(wxCommandEvent& event);
    void OnButtonDel3(wxCommandEvent& event);
    void OnButtonDel4(wxCommandEvent& event);
    void OnButtonDel5(wxCommandEvent& event);
    void OnButtonDel6(wxCommandEvent& event);
    void OnButtonDel7(wxCommandEvent& event);
    void OnButtonDel8(wxCommandEvent& event);
    void OnButtonDel9(wxCommandEvent& event);
    void OnButtonDel10(wxCommandEvent& event);
    void OnButtonDel11(wxCommandEvent& event);
    void OnButtonDel12(wxCommandEvent& event);
    void OnButtonDel13(wxCommandEvent& event);
    void OnButtonDel14(wxCommandEvent& event);
    void OnButtonDel15(wxCommandEvent& event);
    void OnButtonDel16(wxCommandEvent& event);
    void OnButtonDel17(wxCommandEvent& event);
    void OnButtonDel18(wxCommandEvent& event);
    void OnButtonDel19(wxCommandEvent& event);
    void OnButtonAddField(wxCommandEvent& event);
    void OnButtonSend(wxCommandEvent& event);
    void OnButtonPreview(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);

public:
    /** Constructor */
    CEditProductDialog(wxWindow* parent);

    // Custom
    enum { FIELDS_MAX = 20 };
    wxTextCtrl* m_textCtrlLabel[FIELDS_MAX];
    wxTextCtrl* m_textCtrlField[FIELDS_MAX];
    wxButton*   m_buttonDel[FIELDS_MAX];

    void LayoutAll();
    void ShowLine(int i, bool fShow=true);
    void OnButtonDel(wxCommandEvent& event, int n);
    void SetProduct(const CProduct& productIn);
    void GetProduct(CProduct& product);

};



class CViewProductDialog : public CViewProductDialogBase
{
protected:
    // Event handlers
    void OnButtonSubmitForm(wxCommandEvent& event);
    void OnButtonCancelForm(wxCommandEvent& event);
    void OnButtonBack(wxCommandEvent& event);
    void OnButtonNext(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);

public:
    /** Constructor */
    CViewProductDialog(wxWindow* parent, const CProduct& productIn);
    ~CViewProductDialog();

    // Custom
    CProduct product;
    enum { FIELDS_MAX = 20 };
    wxStaticText* m_staticTextLabel[FIELDS_MAX];
    wxTextCtrl*   m_textCtrlField[FIELDS_MAX];
    wxChoice*     m_choiceField[FIELDS_MAX];

    void GetOrder(CWalletTx& order);
    void UpdateProductDisplay(bool fDetails);
    void OnReply1(wxCommandEvent& event);
};



class CViewOrderDialog : public CViewOrderDialogBase
{
protected:
    // Event handlers
    void OnButtonOK(wxCommandEvent& event);

public:
    /** Constructor */
    CViewOrderDialog(wxWindow* parent, CWalletTx order, bool fReceived);

    // Custom
    bool fReceived;
};



class CEditReviewDialog : public CEditReviewDialogBase
{
protected:
    // Event handlers
    void OnKeyDown(wxKeyEvent& event) { HandleCtrlA(event); }
    void OnButtonSubmit(wxCommandEvent& event);
    void OnButtonCancel(wxCommandEvent& event);

public:
    /** Constructor */
    CEditReviewDialog(wxWindow* parent);

    // Custom
    void GetReview(CReview& review);
};



class CGetTextFromUserDialog : public CGetTextFromUserDialogBase
{
protected:
    // Event handlers
    void OnButtonOK(wxCommandEvent& event)     { EndModal(true); }
    void OnButtonCancel(wxCommandEvent& event) { EndModal(false); }
    void OnClose(wxCloseEvent& event)          { EndModal(false); }

    void OnKeyDown(wxKeyEvent& event)
    {
        if (event.GetKeyCode() == '\r' || event.GetKeyCode() == WXK_NUMPAD_ENTER)
            EndModal(true);
        else
            HandleCtrlA(event);
    }

public:
    /** Constructor */
    CGetTextFromUserDialog(wxWindow* parent,
                           const string& strCaption,
                           const string& strMessage1,
                           const string& strValue1="",
                           const string& strMessage2="",
                           const string& strValue2="") : CGetTextFromUserDialogBase(parent, wxID_ANY, strCaption)
    {
        m_staticTextMessage1->SetLabel(strMessage1);
        m_textCtrl1->SetValue(strValue1);
        if (!strMessage2.empty())
        {
            m_staticTextMessage2->Show(true);
            m_staticTextMessage2->SetLabel(strMessage2);
            m_textCtrl2->Show(true);
            m_textCtrl2->SetValue(strValue2);
            SetSize(wxDefaultCoord, 180);
        }
    }

    // Custom
    string GetValue()  { return (string)m_textCtrl1->GetValue(); }
    string GetValue1() { return (string)m_textCtrl1->GetValue(); }
    string GetValue2() { return (string)m_textCtrl2->GetValue(); }
};



class CBitcoinTBIcon : public wxTaskBarIcon
{
protected:
	void Restore();

	// Event handlers
	void OnLeftButtonDClick(wxTaskBarIconEvent&);
	void OnMenuExit(wxCommandEvent&);
	void OnMenuGenerate(wxCommandEvent&);
	void OnMenuRestore(wxCommandEvent&);

public:
	void Show();
	void Hide();
	void UpdateTooltip();
	virtual wxMenu *CreatePopupMenu();

DECLARE_EVENT_TABLE()
};
>>>>>>> dba0fd9b6 (tray icon + ask before closing)
