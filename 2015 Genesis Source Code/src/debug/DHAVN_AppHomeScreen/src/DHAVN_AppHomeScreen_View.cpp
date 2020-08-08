#include <DHAVN_AppHomeScreen_View.h>
#include <QDeclarativeContext>

View::View( int nDisplay, HSDefP::VIEW_ID_T nViewId, QString sViewName, InitData *pInitData, bool bTitleAvailable )
    : m_pInitData( pInitData ),
      m_pModel( new IconsModel( new IconItem ) ),
      m_pSubMenuIconModel (new IconsModel( new IconItem ) ),
      m_nViewId( nViewId ),
      m_nSubViewId ( nViewId ),
      m_sViewName( sViewName ),
      m_sBg( QString() ),
      m_sBGMask( QString() ),
      m_nBgMaskXPos( 0 ),
      m_pCtxt(NULL),
      m_nBgMaskYPos( 0 ),
      m_bTitleAvailable( bTitleAvailable ),
      m_nDisplay( nDisplay ),
      m_bMediaAvailable( !bTitleAvailable ),
      m_nFocusIndex( HSDefP::FOCUS_INDEX_ICONS_MENU ) /** Icons menu */
{
   LoadModel();

   connect( m_pModel, SIGNAL( ReloadModel() ), this, SLOT( LoadModel() ),  Qt::UniqueConnection );
   connect( m_pSubMenuIconModel, SIGNAL( ReloadModel() ), this, SLOT( LoadSubModel() ) , Qt::UniqueConnection );

   m_pModel->SetFocusIndex(m_pModel->rowCount() - 1);
}

View::~View()
{
   disconnect();
   delete m_pModel;
   m_pModel = NULL;

   delete m_pSubMenuIconModel;
   m_pSubMenuIconModel = NULL;
}

bool View::SetBGParameters()
{
   bool bResult = false;

   if( m_pInitData )
   {
      QString sBG = m_pInitData->GetBG( m_pModel->rowCount(), m_pModel->IsMainMenu() );

      if ( m_sBg.compare( sBG ) != 0 )
      {
         bResult = true;

         //! Get BG image path
         m_sBg = sBG;

         //! Get BG Mask image info
         m_sBGMask = m_pInitData->GetBGMask( m_pModel->rowCount(), m_pModel->IsMainMenu() );

         QPoint nBGMaskPosition = m_pInitData->GetBGMaskPosition( m_pModel->rowCount(), m_pModel->IsMainMenu() );

         m_nBgMaskXPos = nBGMaskPosition.x();
         m_nBgMaskYPos = nBGMaskPosition.y();
      }
   }

   return bResult;
}

void View::SetFocusIconIndex( int iconIdx )
{
   if ( m_pModel )
   {
      m_pModel->SetFocusIconIndex( iconIdx );
   }
}

void View::SetFocusIconIndexByViewID( HSDefP::VIEW_ID_T iconID )
{
    if ( m_pModel )
    {
       m_pModel->SetFocusIconIndexByViewID( iconID );
    }
}

void View::SetFocusIconIndexByAppID( HSDefP::APP_ID_T iconID )
{
    if ( m_pModel )
    {
       m_pModel->SetFocusIconIndexByAppID( iconID );
    }
}

int View::GetSubMenuFocusIconIndexByViewID( HSDefP::VIEW_ID_T iconID )
{
    if (m_pSubMenuIconModel)
    {
        return m_pSubMenuIconModel->GetIndexFromViewID(iconID);
    }

    else
    {
        return -1;
    }
}

int View::GetSubMenuFocusIconIndexByAppID( HSDefP::APP_ID_T iconID )
{
    if (m_pSubMenuIconModel)
    {
        return m_pSubMenuIconModel->GetIndexFromAppID(iconID);
    }

    else
    {
        return -1;
    }
}

void View::UpdateSubMenuContext( HSDefP::VIEW_ID_T nView, QDeclarativeContext* pCtxt )
{
    m_nSubViewId = nView;
    m_pCtxt = pCtxt;

    if( ( NULL != m_pSubMenuIconModel ) &&
        ( NULL != m_pInitData ) )
    {
        m_pSubMenuIconModel->Clear();
        m_pInitData->FillViewModel( m_nDisplay, m_pSubMenuIconModel, nView, 0 );
    }

    pCtxt->setContextProperty( "SubMenuListModel", m_pSubMenuIconModel);
    emit updateSubModel();
}

void View::LoadSubModel()
{
    if( ( NULL != m_pSubMenuIconModel ) &&
        ( NULL != m_pInitData ) )
    {
        m_pInitData->FillViewModel( m_nDisplay, m_pSubMenuIconModel, m_nSubViewId, 0 );
    }

    //m_pCtxt->setContextProperty( "SubMenuListModel", m_pSubMenuIconModel);

    emit updateSubModel();
}


void View::SetFocusIndex( int nIndex )
{
   m_nFocusIndex = nIndex;
   emit UpdateFocusedIndex();
}

void View::LoadModel()
{
   if( ( NULL != m_pModel ) &&
       ( NULL != m_pInitData ) )
   {
      m_pInitData->FillViewModel( m_nDisplay, m_pModel, m_nViewId, 0 );

      if ( SetBGParameters() == true )
      {
         emit UpdateBg();
         emit UpdateBgMask();
         emit UpdateBgMaskXPos();
         emit UpdateBGMaskYPos();
      }
   }
}
