#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "v11.h"
#endif

#include "v11Doc.h"
#include "v11View.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cv11View

IMPLEMENT_DYNCREATE(Cv11View, CView)

BEGIN_MESSAGE_MAP(Cv11View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cv11View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SHAPE, &Cv11View::OnShape)
	ON_COMMAND(ID_COLOR, &Cv11View::OnColor)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM,OnHighlightRibbonListItem)
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() {
	shape =0 ;
	color = 0;
	tempshape = 0;
	tempcolor = 0;
}

Cv11View::~Cv11View()
{

}

BOOL Cv11View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cv11View drawing

void Cv11View::OnDraw(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, color);
	pDC->SelectObject(pen);
	switch (shape)
	{
	case 0: pDC->Rectangle(rect);
		break;
	case 1: pDC->Ellipse(rect);
		break;
	case 2: pDC->RoundRect(rect, { 10, 10 });
		break;
	default:
		break;
	}
}


// Cv11View printing


void Cv11View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cv11View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cv11View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cv11View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cv11View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cv11View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cv11View diagnostics

#ifdef _DEBUG
void Cv11View::AssertValid() const
{
	CView::AssertValid();
}

void Cv11View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cv11Doc* Cv11View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cv11Doc)));
	return (Cv11Doc*)m_pDocument;
}
#endif //_DEBUG


// Cv11View message handlers



void Cv11View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRectTracker tract;
	if (tract.TrackRubberBand(this, point))
	{
		rect = tract.m_rect;
		Invalidate();
	}

	//CView::OnLButtonDown(nFlags, point);
}


void Cv11View::OnShape()
{
	// TODO: Add your command handler code here
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>arry;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arry);
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)arry.GetAt(0);
	shape = pGallery->GetSelectedItem();
	tempshape = shape;
	Invalidate();


}


void Cv11View::OnColor()
{
	// TODO: Add your command handler code here
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arry;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arry);
	CMFCRibbonColorButton* cBotton= (CMFCRibbonColorButton*)arry.GetAt(0);
	
		color = cBotton->GetColor();
		tempcolor = color;
		Invalidate();


}
LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wp, LPARAM lp)
{
	int index = (int)wp;
	CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)lp;
	UINT id = pElem->GetID();
	
	if (id == ID_SHAPE)
	{ 
			shape = index;
	}
	if (id == ID_COLOR)
	{
		CMFCRibbonColorButton* pColor = (CMFCRibbonColorButton*)pElem;
		color = pColor->GetHighlightedColor();
	}
		if (index == -1) 
		{
			shape = tempshape;
			color = tempcolor;
			
		}

		Invalidate();
	return 0;
}
