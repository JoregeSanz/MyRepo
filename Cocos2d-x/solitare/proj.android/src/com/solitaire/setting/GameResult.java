package com.solitaire.setting;

public class GameResult {
	public String m_sUsername;
	public String m_sDrawmode;
	public boolean m_bTimed;
	public int m_nScore;
	
	public GameResult(){}
	
	public GameResult(final String sUsername, final String sDrawmode, final String sTimed, final String sScore)
	{
		m_sUsername = sUsername;
		m_sDrawmode = sDrawmode;
		if( sTimed.equals("1"))
			m_bTimed = true;
		else 
			m_bTimed = false;
		
		m_nScore = Integer.parseInt(sScore);
	}
	
	public GameResult(final String sResult)
	{
		String params[] = sResult.split("_");
		
		m_sUsername = params[0];
		m_sDrawmode = params[1];
		
		final String sTimed = params[2];
		if( sTimed.equals("1") )
			m_bTimed = true;
		else
			m_bTimed = false;
		
		m_nScore = Integer.parseInt(params[3]);
	}
	
	public String getResultString()
	{
		String result = m_sUsername + "_" + m_sDrawmode;
		if( m_bTimed )
			result = result + "_1";
		else
			result = result + "_0";
		
		result = result + "_" + String.valueOf(m_nScore);
		
		return result;
	}
}
