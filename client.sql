-- phpMyAdmin SQL Dump
-- version 4.5.4.1deb2ubuntu2.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Creato il: Mar 01, 2020 alle 19:20
-- Versione del server: 5.7.29-0ubuntu0.16.04.1
-- Versione PHP: 7.0.33-0ubuntu0.16.04.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `PROCESSO-6012`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `COATING`
--

CREATE TABLE `COATING` (
  `ID_COATING` int(11) NOT NULL,
  `COATING` char(100) DEFAULT NULL,
  `CAMPO` char(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `COATING`
--

INSERT INTO `COATING` (`ID_COATING`, `COATING`, `CAMPO`) VALUES
(1, 'TiN', 'TiN'),
(2, 'TiCN', 'TiCN'),
(3, 'TiAlN', 'TiAlN'),
(4, 'CRN', 'TiCN'),
(6, 'ALCRN', 'ALCRN'),
(7, 'RED', 'RED');

-- --------------------------------------------------------

--
-- Struttura della tabella `COMP_STEP`
--

CREATE TABLE `COMP_STEP` (
  `ID_COMP_STEP` int(11) NOT NULL,
  `ID_MACCHINA` int(11) NOT NULL DEFAULT '0',
  `COMP_STEP` text,
  `ID_TIPO_STEP` int(11) NOT NULL DEFAULT '1',
  `TMP` tinyint(1) NOT NULL,
  `DESCRIZIONE` varchar(200) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `COMP_STEP`
--

INSERT INTO `COMP_STEP` (`ID_COMP_STEP`, `ID_MACCHINA`, `COMP_STEP`, `ID_TIPO_STEP`, `TMP`, `DESCRIZIONE`) VALUES
(1, 0, 'NESSUNO', 1, 0, 'NESSUNO'),
(3, 0, 'TIN_BASSO', 4, 0, 'TIN_BASSO'),
(4, 0, 'TIN_BASSO_TMP', 4, 0, 'TIN_BASSO_TMP'),
(5, 0, 'ETCHING', 3, 0, 'ETCHING'),
(6, 0, 'TIN_INTERFACCIA', 4, 0, 'TIN_INTERFACCIA'),
(8, 0, 'NERO_INTERFACCIA', 4, 0, 'NERO_INTERFACCIA'),
(9, 0, 'NERO', 4, 0, 'NERO'),
(10, 0, 'TICN_BASSO', 4, 0, 'TICN_BASSO'),
(11, 0, 'TICN_COLORE', 4, 0, 'TICN_COLORE'),
(12, 0, 'PLASMA_ETCHING', 3, 0, 'PLASMA_ETCHING'),
(13, 0, 'GIALLO', 4, 0, 'GIALLO'),
(14, 0, 'TEST_CATODI', 4, 0, 'TEST_CATODI'),
(15, 0, 'PIRONI_CROMO', 4, 0, 'PIRONI_CROMO'),
(16, 0, 'TIN_TIALN_MULTI-2', 4, 0, 'TIN_TIALN_MULTI-2'),
(17, 0, 'CRN', 4, 0, 'CRN'),
(18, 0, 'TIN_PIRONI', 4, 0, 'TIN_PIRONI'),
(19, 0, 'ROSSO-REPIT', 4, 0, 'ROSSO-REPIT'),
(20, 0, 'ROSSO', 4, 0, 'ROSSO'),
(21, 0, 'PULIZIA', 4, 0, 'PULIZIA'),
(22, 0, 'ROSSO_COLORE', 1, 0, 'ROSSO_COLORE'),
(23, 0, 'NERO-COLORE', 1, 0, 'NERO-COLORE'),
(24, 0, 'ETCHING_SOFT', 1, 0, 'ETCHING_SOFT'),
(25, 0, 'TIN_PIRONI', 1, 0, 'TIN_PIRONI'),
(26, 1, 'NESSUNO', 1, 0, 'NESSUNO'),
(27, 1, 'PLASMA_ETCHING_SECONDA', 3, 0, 'PLASMA_ETCHING'),
(28, 1, 'GIALLO-SECONDA', 4, 0, 'GIALLO');

-- --------------------------------------------------------

--
-- Struttura della tabella `CRN`
--

CREATE TABLE `CRN` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `CRN`
--

INSERT INTO `CRN` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 21, 60, 60, 900, '2', 60, 60, 1200, '2', 0, 0, 3600, '2'),
(2, 11, 300, 300, 900, '0', 330, 330, 1200, '0', 300, 300, 3600, '0'),
(3, 30, 80, 80, 900, '0', 80, 80, 1200, '0', 80, 80, 3600, '0'),
(5, 20, 0, 0, 900, '2', 60, 60, 1200, '2', 60, 60, 3600, '2'),
(6, 23, 60, 60, 900, '2', 60, 60, 1200, '2', 0, 0, 3600, '2'),
(7, 22, 0, 0, 900, '2', 60, 60, 1200, '2', 60, 60, 3600, '2');

-- --------------------------------------------------------

--
-- Struttura della tabella `ETCHING`
--

CREATE TABLE `ETCHING` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL,
  `DA4` int(11) DEFAULT NULL,
  `A4` int(11) DEFAULT NULL,
  `TR4` int(11) DEFAULT NULL,
  `INFO4` char(4) DEFAULT NULL,
  `DA5` int(11) DEFAULT NULL,
  `A5` int(11) DEFAULT NULL,
  `TR5` int(11) DEFAULT NULL,
  `INFO5` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `ETCHING`
--

INSERT INTO `ETCHING` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`, `DA4`, `A4`, `TR4`, `INFO4`, `DA5`, `A5`, `TR5`, `INFO5`) VALUES
(1, 10, 80, 80, 25, '0', 80, 80, 85, '0', 80, 80, 115, '0', 80, 80, 175, '0', 80, 80, 190, '0'),
(2, 30, 1000, 1000, 25, '1', 1000, 1000, 85, '1', 1000, 1000, 115, '1', 1000, 1000, 175, '1', 1000, 1000, 190, '1'),
(3, 25, 0, 0, 25, '1', 60, 60, 85, '1', 0, 0, 115, '1', 60, 60, 175, '1', 0, 0, 190, '1'),
(4, 24, 60, 60, 25, '1', 0, 0, 85, '1', 60, 60, 115, '1', 0, 0, 175, '1', 0, 0, 190, '1');

-- --------------------------------------------------------

--
-- Struttura della tabella `ETCHING_SOFT`
--

CREATE TABLE `ETCHING_SOFT` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL,
  `DA4` int(11) DEFAULT NULL,
  `A4` int(11) DEFAULT NULL,
  `TR4` int(11) DEFAULT NULL,
  `INFO4` char(4) DEFAULT NULL,
  `DA5` int(11) DEFAULT NULL,
  `A5` int(11) DEFAULT NULL,
  `TR5` int(11) DEFAULT NULL,
  `INFO5` char(4) DEFAULT NULL,
  `DA6` int(11) DEFAULT NULL,
  `A6` int(11) DEFAULT NULL,
  `TR6` int(11) DEFAULT NULL,
  `INFO6` char(4) DEFAULT NULL,
  `DA7` int(11) DEFAULT NULL,
  `A7` int(11) DEFAULT NULL,
  `TR7` int(11) DEFAULT NULL,
  `INFO7` char(4) DEFAULT NULL,
  `DA8` int(11) DEFAULT NULL,
  `A8` int(11) DEFAULT NULL,
  `TR8` int(11) DEFAULT NULL,
  `INFO8` char(4) DEFAULT NULL,
  `DA9` int(11) DEFAULT NULL,
  `A9` int(11) DEFAULT NULL,
  `TR9` int(11) DEFAULT NULL,
  `INFO9` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `ETCHING_SOFT`
--

INSERT INTO `ETCHING_SOFT` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`, `DA4`, `A4`, `TR4`, `INFO4`, `DA5`, `A5`, `TR5`, `INFO5`, `DA6`, `A6`, `TR6`, `INFO6`, `DA7`, `A7`, `TR7`, `INFO7`, `DA8`, `A8`, `TR8`, `INFO8`, `DA9`, `A9`, `TR9`, `INFO9`) VALUES
(1, 10, 80, 80, 10, '0', 80, 80, 25, '0', 80, 80, 45, '0', 80, 80, 60, '0', 80, 80, 80, '0', 80, 80, 95, '0', 80, 80, 115, '0', 80, 80, 130, '0', 80, 80, 140, '0'),
(2, 30, 800, 800, 10, '1', 800, 800, 25, '1', 900, 900, 45, '1', 900, 900, 60, '1', 1000, 1000, 80, '1', 1000, 1000, 95, '1', 1000, 1000, 115, '1', 1000, 1000, 130, '1', 1000, 1000, 140, '1'),
(3, 25, 0, 0, 10, '1', 0, 0, 25, '1', 0, 0, 45, '1', 0, 0, 60, '1', 0, 0, 80, '1', 0, 0, 95, '1', 0, 0, 115, '1', 0, 0, 130, '1', 60, 60, 140, '1'),
(4, 24, 60, 60, 10, '1', 0, 0, 25, '1', 60, 60, 45, '1', 0, 0, 60, '1', 60, 60, 80, '1', 0, 0, 95, '1', 60, 60, 115, '1', 0, 0, 130, '1', 0, 0, 140, '1');

-- --------------------------------------------------------

--
-- Struttura della tabella `GIALLO`
--

CREATE TABLE `GIALLO` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL,
  `DA4` int(11) DEFAULT NULL,
  `A4` int(11) DEFAULT NULL,
  `TR4` int(11) DEFAULT NULL,
  `INFO4` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `GIALLO`
--

INSERT INTO `GIALLO` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`, `DA4`, `A4`, `TR4`, `INFO4`) VALUES
(1, 21, 60, 60, 300, '2', 60, 60, 1500, '2', 60, 60, 2700, '2', 0, 0, 2760, '2'),
(2, 11, 250, 250, 300, '0', 250, 250, 1500, '0', 250, 250, 2700, '0', 250, 250, 2760, '0'),
(3, 30, 80, 80, 300, '0', 80, 80, 1500, '0', 100, 100, 2700, '0', 100, 100, 2760, '0'),
(5, 20, 60, 60, 300, '2', 60, 60, 1500, '2', 60, 60, 2700, '2', 0, 0, 2760, '2'),
(6, 23, 60, 60, 300, '2', 60, 60, 1500, '2', 60, 60, 2700, '2', 0, 0, 2760, '2'),
(7, 22, 60, 60, 300, '2', 60, 60, 1500, '2', 60, 60, 2700, '2', 0, 0, 2760, '2');

-- --------------------------------------------------------

--
-- Struttura della tabella `GIALLO-SECONDA`
--

CREATE TABLE `GIALLO-SECONDA` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `GIALLO-SECONDA`
--

INSERT INTO `GIALLO-SECONDA` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`) VALUES
(3, 30, 80, 80, 3300, '0'),
(5, 21, 55, 55, 3300, '1'),
(6, 22, 55, 55, 3300, '1'),
(7, 25, 55, 55, 3300, '1'),
(8, 11, 200, 200, 3300, '140');

-- --------------------------------------------------------

--
-- Struttura della tabella `NERO`
--

CREATE TABLE `NERO` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `NERO`
--

INSERT INTO `NERO` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 23, 55, 55, 360, '2', 55, 55, 1500, '2', 55, 55, 2100, '2'),
(2, 30, 50, 50, 360, '0', 50, 50, 1500, '0', 60, 60, 2100, '0'),
(3, 22, 55, 55, 360, '2', 55, 55, 1500, '2', 55, 55, 2100, '2'),
(4, 20, 55, 55, 360, '2', 55, 55, 1500, '2', 55, 55, 2100, '2'),
(5, 11, 380, 380, 360, '180', 380, 380, 1500, '180', 380, 380, 2100, '180'),
(6, 21, 55, 55, 360, '2', 55, 55, 1500, '2', 55, 55, 2100, '2');

-- --------------------------------------------------------

--
-- Struttura della tabella `NERO-COLORE`
--

CREATE TABLE `NERO-COLORE` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `NERO-COLORE`
--

INSERT INTO `NERO-COLORE` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 23, 55, 55, 180, '2', 55, 55, 360, '2', 55, 55, 480, '2'),
(2, 30, 50, 50, 180, '0', 50, 50, 360, '0', 90, 90, 480, '0'),
(3, 22, 55, 55, 180, '2', 55, 55, 360, '2', 55, 55, 480, '2'),
(4, 20, 55, 55, 180, '2', 55, 55, 360, '2', 55, 55, 480, '2'),
(5, 11, 300, 300, 180, '180', 300, 300, 360, '180', 300, 300, 480, '180'),
(6, 21, 55, 55, 180, '2', 55, 55, 360, '2', 55, 55, 480, '2');

-- --------------------------------------------------------

--
-- Struttura della tabella `NERO_INTERFACCIA`
--

CREATE TABLE `NERO_INTERFACCIA` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `NERO_INTERFACCIA`
--

INSERT INTO `NERO_INTERFACCIA` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`) VALUES
(1, 11, 300, 300, 60, '150', 300, 300, 180, '150'),
(2, 30, 70, 70, 60, '0', 70, 70, 180, '0'),
(3, 22, 56, 56, 60, '1', 56, 56, 180, '1'),
(4, 20, 56, 56, 60, '1', 56, 56, 180, '1'),
(5, 24, 60, 60, 60, '1', 60, 60, 180, '1'),
(6, 25, 60, 60, 60, '1', 60, 60, 180, '1');

-- --------------------------------------------------------

--
-- Struttura della tabella `PIRONI_CROMO`
--

CREATE TABLE `PIRONI_CROMO` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `PIRONI_CROMO`
--

INSERT INTO `PIRONI_CROMO` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 21, 60, 60, 300, '2', 60, 60, 360, '2', 60, 60, 900, '2'),
(2, 11, 250, 250, 300, '0', 250, 250, 360, '0', 250, 250, 900, '0'),
(3, 30, 100, 100, 300, '0', 100, 100, 360, '0', 130, 130, 900, '0'),
(5, 22, 60, 60, 300, '2', 60, 60, 360, '2', 60, 60, 900, '2'),
(6, 20, 60, 60, 300, '2', 60, 60, 360, '2', 60, 60, 900, '2');

-- --------------------------------------------------------

--
-- Struttura della tabella `PLASMA_ETCHING`
--

CREATE TABLE `PLASMA_ETCHING` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `PLASMA_ETCHING`
--

INSERT INTO `PLASMA_ETCHING` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 10, 90, 90, 10, '0', 90, 90, 130, '0', 90, 90, 190, '0'),
(2, 30, 500, 500, 10, '1', 600, 600, 130, '1', 700, 700, 190, '1'),
(3, 24, 55, 55, 10, '2', 55, 55, 130, '2', 0, 0, 190, '2'),
(4, 25, 0, 0, 10, '2', 0, 0, 130, '2', 55, 55, 190, '2');

-- --------------------------------------------------------

--
-- Struttura della tabella `PLASMA_ETCHING_SECONDA`
--

CREATE TABLE `PLASMA_ETCHING_SECONDA` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `PLASMA_ETCHING_SECONDA`
--

INSERT INTO `PLASMA_ETCHING_SECONDA` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`) VALUES
(1, 10, 150, 150, 70, '0', 150, 150, 205, '0'),
(2, 30, 400, 400, 70, '1', 400, 400, 205, '1'),
(3, 21, 55, 55, 70, '2', 55, 55, 205, '2'),
(4, 22, 0, 0, 70, '2', 1, 1, 205, '2'),
(5, 60, 1, 1, 70, '0', 0, 0, 205, '0');

-- --------------------------------------------------------

--
-- Struttura della tabella `PROCESSO`
--

CREATE TABLE `PROCESSO` (
  `ID_PROCESSO` int(11) NOT NULL,
  `ID_MACCHINA` int(11) NOT NULL DEFAULT '0',
  `PROCESSO` text,
  `ID_COATING` int(11) NOT NULL,
  `TARGET_0` tinyint(4) NOT NULL DEFAULT '1',
  `TARGET_1` tinyint(4) NOT NULL DEFAULT '1',
  `TARGET_2` tinyint(4) NOT NULL DEFAULT '1',
  `TARGET_3` tinyint(4) NOT NULL DEFAULT '1',
  `TARGET_4` tinyint(4) NOT NULL DEFAULT '1',
  `TARGET_5` tinyint(4) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `PROCESSO`
--

INSERT INTO `PROCESSO` (`ID_PROCESSO`, `ID_MACCHINA`, `PROCESSO`, `ID_COATING`, `TARGET_0`, `TARGET_1`, `TARGET_2`, `TARGET_3`, `TARGET_4`, `TARGET_5`) VALUES
(3, 0, 'NERO', 3, 2, 2, 2, 2, 1, 1),
(4, 0, 'TIN_BASSO_NOTARIS', 1, 1, 1, 1, 1, 1, 1),
(5, 0, 'TICN_BASSO_TMP', 2, 1, 1, 1, 1, 1, 1),
(6, 0, 'GIALLO', 1, 1, 1, 1, 1, 1, 1),
(7, 0, 'RISCALDO', 1, 0, 0, 0, 0, 0, 0),
(8, 0, 'TEST_CATODI', 0, 1, 1, 1, 1, 1, 1),
(9, 0, 'PIRONI_CROMO', 4, 4, 0, 4, 0, 1, 1),
(11, 0, 'PIRONI_TIN', 1, 1, 1, 1, 1, 1, 1),
(12, 0, 'ROSSO', 7, 2, 1, 2, 1, 1, 1),
(13, 0, 'PULIZIA CATODI', 0, 1, 1, 1, 1, 1, 1),
(14, 0, 'NERO-COLORE', 3, 2, 2, 2, 2, 2, 1),
(15, 0, 'ROSSO-COLORE', 7, 2, 1, 2, 1, 1, 1),
(16, 1, 'RISCALDO', 1, 0, 0, 0, 0, 0, 0),
(17, 1, 'GIALLO', 1, 0, 1, 0, 1, 0, 1);

-- --------------------------------------------------------

--
-- Struttura della tabella `PULIZIA`
--

CREATE TABLE `PULIZIA` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `PULIZIA`
--

INSERT INTO `PULIZIA` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`) VALUES
(1, 11, 340, 340, 300, '160'),
(2, 30, 200, 200, 300, '0'),
(3, 22, 60, 60, 300, '1'),
(4, 20, 60, 60, 300, '1'),
(7, 21, 60, 60, 300, '1'),
(8, 23, 60, 60, 300, '1');

-- --------------------------------------------------------

--
-- Struttura della tabella `ROSSO`
--

CREATE TABLE `ROSSO` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` text,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `ROSSO`
--

INSERT INTO `ROSSO` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 30, 100, 100, 180, '0', 100, 100, 240, '0', 100, 100, 480, '0'),
(2, 11, 300, 300, 180, '0', 290, 270, 240, '0', 270, 270, 480, '0'),
(3, 12, 0, 0, 180, NULL, 10, 30, 240, NULL, 30, 30, 480, NULL),
(4, 22, 56, 56, 180, '1', 56, 56, 240, '1', 56, 56, 480, '1'),
(5, 20, 58, 58, 180, '1', 58, 58, 240, '1', 58, 58, 480, '1'),
(6, 21, 63, 63, 180, '1', 63, 63, 240, '1', 63, 63, 480, '1'),
(7, 23, 63, 63, 180, '1', 63, 63, 240, '1', 63, 63, 480, '1');

-- --------------------------------------------------------

--
-- Struttura della tabella `ROSSO-REPIT`
--

CREATE TABLE `ROSSO-REPIT` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL,
  `DA4` int(11) DEFAULT NULL,
  `A4` int(11) DEFAULT NULL,
  `TR4` int(11) DEFAULT NULL,
  `INFO4` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `ROSSO-REPIT`
--

INSERT INTO `ROSSO-REPIT` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`, `DA4`, `A4`, `TR4`, `INFO4`) VALUES
(1, 23, 63, 63, 180, '2', 63, 63, 240, '2', 0, 0, 540, '0', 63, 63, 600, '0'),
(2, 30, 120, 120, 180, '0', 120, 120, 240, '0', 120, 120, 540, '0', 120, 120, 600, '0'),
(3, 22, 0, 0, 180, '2', 56, 56, 240, '2', 56, 56, 540, '0', 56, 56, 600, '0'),
(4, 20, 0, 0, 180, '2', 58, 58, 240, '2', 58, 58, 540, '0', 58, 58, 600, '0'),
(5, 11, 340, 340, 180, '150', 380, 380, 240, '150', 340, 347, 540, '180', 380, 380, 600, '150'),
(6, 21, 63, 63, 180, '2', 63, 63, 240, '2', 0, 0, 540, '0', 63, 63, 600, NULL);

-- --------------------------------------------------------

--
-- Struttura della tabella `ROSSO_COLORE`
--

CREATE TABLE `ROSSO_COLORE` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` char(4) DEFAULT NULL,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `ROSSO_COLORE`
--

INSERT INTO `ROSSO_COLORE` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 21, 60, 60, 180, '2', 60, 60, 300, '2', 60, 60, 720, '2'),
(2, 11, 340, 340, 180, '0', 332, 332, 300, '0', 332, 332, 720, '0'),
(3, 30, 80, 80, 180, '0', 80, 80, 300, '0', 80, 80, 720, '0'),
(4, 23, 60, 60, 180, '2', 60, 60, 300, '2', 60, 60, 720, '2'),
(5, 12, 0, 0, 180, '0', 8, 8, 300, '0', 8, 8, 720, '0');

-- --------------------------------------------------------

--
-- Struttura della tabella `STEP`
--

CREATE TABLE `STEP` (
  `ID_STEP` int(11) NOT NULL,
  `ID_PROCESSO` int(11) DEFAULT NULL,
  `POS` int(11) DEFAULT NULL,
  `ID_TIPO_STEP` int(11) DEFAULT NULL,
  `TEMPO` int(11) DEFAULT NULL,
  `REPIT` int(11) DEFAULT '1',
  `PRESSIONE` double DEFAULT NULL,
  `POTENZA` int(11) DEFAULT NULL,
  `TEMP` int(11) DEFAULT NULL,
  `BIAS_OFF_TIME` tinyint(4) NOT NULL DEFAULT '0',
  `TAVOLA` int(11) DEFAULT NULL,
  `SORGENTE_ON` char(2) DEFAULT NULL,
  `ID_COMP_STEP` int(11) DEFAULT '1',
  `TMP` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `STEP`
--

INSERT INTO `STEP` (`ID_STEP`, `ID_PROCESSO`, `POS`, `ID_TIPO_STEP`, `TEMPO`, `REPIT`, `PRESSIONE`, `POTENZA`, `TEMP`, `BIAS_OFF_TIME`, `TAVOLA`, `SORGENTE_ON`, `ID_COMP_STEP`, `TMP`) VALUES
(26, 3, 1, 1, 30, 1, 3, 0, 0, 0, 0, '', 1, 0),
(27, 3, 2, 2, 3600, 1, 3, 100, 450, 0, 0, '', 1, 0),
(28, 3, 3, 2, 1800, 1, 2, 90, 450, 0, 0, '', 1, 0),
(29, 3, 4, 2, 1200, 1, 0.55, 70, 400, 0, 0, '', 1, 0),
(31, 3, 8, 4, 180, 1, 0, 0, 0, 0, 0, '', 6, 0),
(33, 3, 10, 4, 2100, 1, 0, 0, 0, 0, 0, '', 9, 0),
(35, 4, 1, 1, 10, 1, 1, 0, 0, 0, 0, '', 1, 0),
(36, 4, 2, 2, 2700, 1, 1, 100, 400, 0, 0, '', 1, 0),
(37, 4, 3, 2, 1200, 1, 1, 90, 400, 0, 0, '', 1, 0),
(38, 4, 5, 3, 190, 2, 0.4, 70, 350, 0, 0, '', 12, 0),
(39, 4, 6, 3, 190, 3, 0, 0, 0, 0, 0, '', 5, 0),
(40, 4, 7, 4, 600, 1, 0, 0, 0, 0, 0, '', 6, 0),
(41, 4, 8, 5, 10, 1, 0.03, 0, 0, 0, 0, '', 1, 0),
(42, 5, 1, 1, 60, 1, 1, 0, 0, 0, NULL, NULL, 1, 0),
(43, 5, 2, 2, 2700, 1, 1, 100, 400, 0, NULL, NULL, 1, 0),
(44, 5, 3, 2, 1800, 1, 1, 90, 350, 0, NULL, NULL, 1, 0),
(45, 5, 4, 4, 600, 1, 1, 0, 0, 0, NULL, NULL, 6, 0),
(47, 5, 6, 3, 190, 5, 1, 0, 0, 0, NULL, NULL, 5, 0),
(48, 5, 5, 2, 1200, 1, 0.8, 70, 350, 0, NULL, NULL, 1, 0),
(51, 5, 7, 5, 60, 1, 1, 0, 0, 0, NULL, NULL, 1, 0),
(52, 5, 8, 4, 840, 1, 1, 0, 0, 0, NULL, NULL, 10, 0),
(53, 5, 9, 4, 480, 1, 1, 0, 0, 0, NULL, NULL, 11, 0),
(55, 6, 2, 2, 3600, 1, 3, 100, 450, 0, 0, '', 1, 0),
(56, 6, 3, 2, 1800, 1, 1.6, 90, 450, 0, 0, '', 1, 0),
(57, 6, 4, 2, 1200, 1, 0.55, 70, 350, 0, 0, '', 1, 0),
(59, 6, 8, 4, 2760, 1, 0, 0, 0, 0, 0, '', 13, 0),
(61, 7, 1, 1, 30, 1, 1, 0, 0, 0, 0, '', 1, 0),
(62, 7, 2, 2, 1200, 1, 1.2, 100, 400, 0, 0, '', 1, 0),
(63, 7, 3, 2, 1200, 1, 1, 90, 350, 0, 0, '', 1, 0),
(64, 7, 4, 2, 600, 1, 0.5, 70, 300, 0, 0, '', 1, 0),
(67, 7, 5, 5, 600, 1, 0.03, 0, 0, 0, 0, '', 1, 0),
(68, 8, 1, 1, 30, 1, 1, 0, 0, 0, 0, '', 1, 0),
(71, 8, 2, 2, 30, 1, 18, 70, 400, 0, 0, '', 1, 0),
(73, 8, 3, 4, 1500, 1, 0, 0, 0, 0, 0, '', 14, 0),
(75, 6, 7, 3, 190, 6, 0, 0, 0, 50, 0, '', 5, 0),
(77, 9, 1, 1, 60, 1, 1, 0, 0, 0, 0, '', 1, 0),
(78, 9, 2, 2, 2700, 1, 4, 90, 400, 0, 0, '', 1, 0),
(79, 9, 3, 2, 1200, 1, 4, 90, 400, 0, 0, '', 1, 0),
(80, 9, 4, 2, 600, 1, 1, 70, 350, 0, 0, '', 1, 0),
(82, 9, 6, 4, 900, 1, 0, 0, 0, 0, 0, '', 18, 0),
(83, 9, 7, 5, 300, 1, 0.03, 0, 0, 0, 0, '', 1, 0),
(84, 9, 5, 3, 160, 3, 0, 0, 0, 0, 0, '', 7, 0),
(88, 4, 4, 2, 1200, 1, 0.3, 0, 0, 0, NULL, NULL, 1, 0),
(89, 10, 1, 1, 60, 1, 1, 0, 0, 0, 0, '', 1, 0),
(90, 10, 2, 2, 60, 1, 4, 100, 350, 0, 0, '', 1, 0),
(92, 10, 3, 2, 60, 1, 0.5, 70, 300, 0, 0, '', 1, 0),
(93, 10, 4, 3, 190, 1, 0, 0, 0, 30, 0, '', 12, 0),
(94, 10, 6, 4, 3600, 1, 0, 0, 0, 0, 0, '', 17, 0),
(95, 10, 7, 5, 60, 1, 0.03, 0, 0, 0, 0, '', 1, 0),
(96, 10, 5, 3, 190, 4, 0, 0, 0, 30, 0, '', 5, 0),
(97, 11, 1, 1, 30, 1, 3, 0, 0, 0, 0, '', 1, 0),
(98, 11, 2, 2, 1800, 1, 7, 100, 400, 0, 0, '', 1, 0),
(99, 11, 3, 2, 1200, 1, 7, 90, 350, 0, 0, '', 1, 0),
(100, 11, 4, 2, 600, 1, 0.8, 70, 350, 0, 0, '', 1, 0),
(102, 11, 8, 4, 600, 1, 0, 0, 0, 0, 0, '', 18, 0),
(105, 11, 5, 3, 190, 1, 0, 0, 0, 30, 0, '', 12, 0),
(106, 11, 7, 3, 190, 4, 0, 0, 0, 30, 0, '', 5, 0),
(107, 12, 1, 1, 30, 1, 3, 0, 0, 0, 0, '', 1, 0),
(108, 12, 2, 2, 3600, 1, 3, 100, 450, 0, 0, '', 1, 0),
(109, 12, 3, 2, 1800, 1, 2, 90, 400, 0, 0, '', 1, 0),
(110, 12, 4, 2, 1200, 1, 0.5, 70, 350, 0, 0, '', 1, 0),
(111, 12, 5, 3, 190, 1, 0, 0, 0, 0, 0, '', 12, 0),
(112, 12, 7, 4, 180, 1, 0, 0, 0, 0, 0, '', 6, 0),
(113, 12, 8, 4, 600, 4, 0, 0, 0, 0, 0, '', 19, 0),
(114, 12, 9, 4, 720, 1, 0, 0, 0, 0, 0, '', 22, 0),
(115, 12, 10, 5, 300, 1, 0, 0, 0, 0, 0, '', 1, 0),
(116, 12, 6, 3, 190, 4, 0, 0, 0, 0, 0, '', 5, 0),
(117, 8, 4, 5, 100, 1, 0.3, 0, 0, 0, NULL, NULL, 1, 0),
(118, 13, 1, 1, 60, 1, 1, 0, 0, 0, 0, '', 1, 0),
(120, 13, 2, 4, 300, 1, 0, 0, 0, 0, 0, '', 21, 0),
(122, 13, 3, 1, 100, 1, 0.3, 0, 0, 0, NULL, NULL, 1, 0),
(124, 14, 1, 1, 30, 1, 3, 0, 0, 0, 0, '', 1, 0),
(125, 14, 2, 2, 600, 1, 4, 100, 450, 0, 0, '', 1, 0),
(126, 14, 3, 2, 600, 1, 2, 90, 400, 0, 0, '', 1, 0),
(127, 14, 4, 2, 300, 1, 1, 70, 350, 0, 0, '', 1, 0),
(132, 14, 11, 5, 300, 1, 0, 0, 0, 0, 0, '', 1, 0),
(134, 6, 9, 5, 600, 1, 0.3, 0, 0, 0, NULL, NULL, 1, 0),
(137, 3, 7, 3, 190, 6, 0, 0, 0, 20, NULL, NULL, 5, 0),
(138, 3, 11, 5, 300, 1, 0.3, 0, 0, 0, NULL, NULL, 1, 0),
(139, 15, 1, 1, 30, 1, 3, 0, 0, 0, 0, '', 1, 0),
(140, 15, 2, 2, 1200, 1, 8, 100, 450, 0, 0, '', 1, 0),
(141, 15, 3, 2, 600, 1, 8, 90, 400, 0, 0, '', 1, 0),
(142, 15, 5, 2, 600, 1, 8, 70, 350, 0, 0, '', 1, 0),
(143, 15, 6, 3, 300, 1, 0, 0, 0, 0, 0, '', 12, 0),
(144, 15, 8, 4, 120, 1, 0, 0, 0, 0, 0, '', 6, 0),
(146, 15, 9, 4, 660, 1, 0, 0, 0, 0, 0, '', 22, 0),
(147, 15, 10, 5, 300, 1, 0, 0, 0, 0, 0, '', 1, 0),
(148, 15, 7, 3, 175, 1, 0, 0, 0, 0, 0, '', 5, 0),
(154, 15, 4, 2, 600, 1, 8, 90, 400, 0, 0, '', 1, 0),
(155, 6, 1, 1, 60, 1, 2, 0, 0, 0, NULL, NULL, 1, 0),
(156, 11, 9, 5, 10, 1, 0.3, 0, 0, 0, NULL, NULL, 1, 0),
(158, 6, 5, 3, 190, 1, 0, 0, 0, 30, 0, '', 12, 0),
(159, 6, 6, 3, 140, 1, 0, 0, 0, 50, 0, '', 24, 0),
(160, 11, 6, 3, 140, 1, 0, 0, 0, 30, 0, '', 24, 0),
(162, 3, 5, 3, 190, 1, 0, 0, 0, 30, NULL, NULL, 12, 0),
(165, 14, 9, 4, 180, 1, 0, 0, 0, 0, 0, '', 8, 0),
(166, 3, 6, 3, 140, 1, 0, 0, 0, 30, NULL, NULL, 24, 0),
(167, 3, 9, 4, 180, 1, 0, 0, 0, 0, 0, '', 8, 0),
(170, 14, 8, 4, 180, 1, 0, 0, 0, 0, 0, '', 6, 0),
(171, 14, 5, 3, 190, 1, 0, 0, 0, 30, 0, '', 12, 0),
(172, 14, 7, 3, 190, 4, 0, 0, 0, 20, 0, '', 5, 0),
(173, 14, 6, 3, 140, 1, 0, 0, 0, 50, 0, '', 24, 0),
(174, 14, 10, 4, 480, 1, 0, 0, 0, 0, 0, '', 23, 0),
(175, 16, 1, 1, 30, 1, 1, 0, 0, 0, 0, '', 26, 0),
(176, 16, 2, 2, 1200, 1, 1.2, 100, 400, 0, 0, '', 26, 0),
(177, 16, 3, 2, 1200, 1, 1, 90, 350, 0, 0, '', 26, 0),
(178, 16, 4, 2, 600, 1, 0.5, 70, 300, 0, 0, '', 26, 0),
(179, 16, 5, 5, 600, 1, 0.03, 0, 0, 0, 0, '', 26, 0),
(180, 17, 1, 1, 30, 1, 3, 0, 0, 0, 0, '', 26, 0),
(181, 17, 2, 2, 60, 1, 7, 100, 400, 0, 0, '', 26, 0),
(182, 17, 3, 2, 60, 1, 7, 90, 350, 0, 0, '', 26, 0),
(183, 17, 4, 3, 205, 1, 3, 70, 350, 100, 0, '', 27, 0),
(184, 17, 5, 4, 3300, 1, 0, 0, 0, 30, 0, '', 28, 0),
(185, 17, 8, 4, 600, 1, 0, 0, 0, 0, 0, '', 18, 0),
(186, 17, 6, 5, 10, 1, 0.3, 0, 0, 0, NULL, NULL, 26, 0);

-- --------------------------------------------------------

--
-- Struttura della tabella `TEST_CATODI`
--

CREATE TABLE `TEST_CATODI` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` text,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `TEST_CATODI`
--

INSERT INTO `TEST_CATODI` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 11, 400, 400, 120, '0', 400, 400, 900, '0', 400, 400, 1500, '0'),
(2, 30, 50, 50, 120, '0', 50, 50, 900, '0', 80, 80, 1500, '0'),
(6, 20, 55, 55, 120, '1', 55, 55, 900, '1', 55, 55, 1500, '1'),
(8, 21, 55, 55, 120, '1', 55, 55, 900, '1', 55, 55, 1500, '1'),
(9, 23, 55, 55, 120, '1', 55, 55, 900, '1', 55, 55, 1500, '1'),
(10, 22, 55, 55, 120, '1', 55, 55, 900, '1', 55, 55, 1500, '1');

-- --------------------------------------------------------

--
-- Struttura della tabella `TICN_BASSO`
--

CREATE TABLE `TICN_BASSO` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` text,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `TICN_BASSO`
--

INSERT INTO `TICN_BASSO` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 21, 60, 60, 300, '2', 60, 60, 540, '2', 60, 60, 840, '2'),
(2, 11, 340, 340, 300, '0', 340, 285, 540, '0', 285, 285, 840, '0'),
(3, 30, 80, 80, 300, '0', 80, 80, 540, '0', 80, 80, 840, '0'),
(4, 23, 60, 60, 300, '2', 60, 60, 540, '2', 60, 60, 840, '2'),
(5, 12, 0, 0, 300, '0', 17, 55, 540, '0', 55, 55, 840, '0');

-- --------------------------------------------------------

--
-- Struttura della tabella `TICN_COLORE`
--

CREATE TABLE `TICN_COLORE` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL,
  `DA2` int(11) DEFAULT NULL,
  `A2` int(11) DEFAULT NULL,
  `TR2` int(11) DEFAULT NULL,
  `INFO2` text,
  `DA3` int(11) DEFAULT NULL,
  `A3` int(11) DEFAULT NULL,
  `TR3` int(11) DEFAULT NULL,
  `INFO3` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `TICN_COLORE`
--

INSERT INTO `TICN_COLORE` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`, `DA2`, `A2`, `TR2`, `INFO2`, `DA3`, `A3`, `TR3`, `INFO3`) VALUES
(1, 21, 60, 60, 180, '2', 60, 60, 300, '2', 60, 60, 480, '2'),
(2, 11, 230, 230, 180, '0', 220, 210, 300, '0', 210, 210, 480, '0'),
(3, 30, 80, 80, 180, '0', 80, 80, 300, '0', 80, 80, 480, '0'),
(4, 23, 60, 60, 180, '2', 60, 60, 300, '2', 60, 60, 480, '2'),
(5, 12, 0, 0, 180, '0', 10, 25, 300, '0', 25, 25, 480, '0');

-- --------------------------------------------------------

--
-- Struttura della tabella `TIN_INTERFACCIA`
--

CREATE TABLE `TIN_INTERFACCIA` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `TIN_INTERFACCIA`
--

INSERT INTO `TIN_INTERFACCIA` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`) VALUES
(2, 11, 300, 300, 180, '140'),
(3, 30, 80, 80, 180, '0'),
(5, 24, 60, 60, 180, '1'),
(6, 25, 60, 60, 180, '1');

-- --------------------------------------------------------

--
-- Struttura della tabella `TIN_PIRONI`
--

CREATE TABLE `TIN_PIRONI` (
  `ID` int(11) NOT NULL,
  `ID_TIPO_COMP` int(11) DEFAULT NULL,
  `DA1` int(11) DEFAULT NULL,
  `A1` int(11) DEFAULT NULL,
  `TR1` int(11) DEFAULT NULL,
  `INFO1` char(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `TIN_PIRONI`
--

INSERT INTO `TIN_PIRONI` (`ID`, `ID_TIPO_COMP`, `DA1`, `A1`, `TR1`, `INFO1`) VALUES
(3, 30, 80, 80, 600, '0'),
(5, 24, 63, 63, 600, '1'),
(6, 25, 63, 63, 600, '1'),
(7, 11, 300, 300, 600, '140');

-- --------------------------------------------------------

--
-- Struttura della tabella `TIPO_COMP`
--

CREATE TABLE `TIPO_COMP` (
  `ID_TIPO_COMP` int(11) NOT NULL,
  `TIPO_COMP` text,
  `COD_COMP` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `TIPO_COMP`
--

INSERT INTO `TIPO_COMP` (`ID_TIPO_COMP`, `TIPO_COMP`, `COD_COMP`) VALUES
(10, 'Argon', 10),
(11, 'Azoto', 11),
(12, 'Aceti', 12),
(20, 'SorSxP', 20),
(21, 'SorDxP', 21),
(22, 'SorDxC', 22),
(23, 'SorSxC', 23),
(24, 'SorMDx', 24),
(25, 'SorMSx', 25),
(30, 'Bias', 30),
(60, 'SchermoTurbo', 60);

-- --------------------------------------------------------

--
-- Struttura della tabella `TIPO_STEP`
--

CREATE TABLE `TIPO_STEP` (
  `ID_TIPO_STEP` int(11) NOT NULL,
  `TIPO_STEP` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `TIPO_STEP`
--

INSERT INTO `TIPO_STEP` (`ID_TIPO_STEP`, `TIPO_STEP`) VALUES
(1, 'Vuoto'),
(2, 'Riscaldo'),
(3, 'Etching'),
(4, 'Coating'),
(5, 'Venting');

--
-- Indici per le tabelle scaricate
--

--
-- Indici per le tabelle `COATING`
--
ALTER TABLE `COATING`
  ADD PRIMARY KEY (`ID_COATING`);

--
-- Indici per le tabelle `COMP_STEP`
--
ALTER TABLE `COMP_STEP`
  ADD PRIMARY KEY (`ID_COMP_STEP`);

--
-- Indici per le tabelle `CRN`
--
ALTER TABLE `CRN`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `ETCHING`
--
ALTER TABLE `ETCHING`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `ETCHING_SOFT`
--
ALTER TABLE `ETCHING_SOFT`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `GIALLO`
--
ALTER TABLE `GIALLO`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `GIALLO-SECONDA`
--
ALTER TABLE `GIALLO-SECONDA`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `NERO`
--
ALTER TABLE `NERO`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `NERO-COLORE`
--
ALTER TABLE `NERO-COLORE`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `NERO_INTERFACCIA`
--
ALTER TABLE `NERO_INTERFACCIA`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `PIRONI_CROMO`
--
ALTER TABLE `PIRONI_CROMO`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `PLASMA_ETCHING`
--
ALTER TABLE `PLASMA_ETCHING`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `PLASMA_ETCHING_SECONDA`
--
ALTER TABLE `PLASMA_ETCHING_SECONDA`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `PROCESSO`
--
ALTER TABLE `PROCESSO`
  ADD PRIMARY KEY (`ID_PROCESSO`);

--
-- Indici per le tabelle `PULIZIA`
--
ALTER TABLE `PULIZIA`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `ROSSO`
--
ALTER TABLE `ROSSO`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `ROSSO-REPIT`
--
ALTER TABLE `ROSSO-REPIT`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `ROSSO_COLORE`
--
ALTER TABLE `ROSSO_COLORE`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `STEP`
--
ALTER TABLE `STEP`
  ADD PRIMARY KEY (`ID_STEP`);

--
-- Indici per le tabelle `TEST_CATODI`
--
ALTER TABLE `TEST_CATODI`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `TICN_BASSO`
--
ALTER TABLE `TICN_BASSO`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `TICN_COLORE`
--
ALTER TABLE `TICN_COLORE`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `TIN_INTERFACCIA`
--
ALTER TABLE `TIN_INTERFACCIA`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `TIN_PIRONI`
--
ALTER TABLE `TIN_PIRONI`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `TIPO_COMP`
--
ALTER TABLE `TIPO_COMP`
  ADD PRIMARY KEY (`ID_TIPO_COMP`);

--
-- Indici per le tabelle `TIPO_STEP`
--
ALTER TABLE `TIPO_STEP`
  ADD PRIMARY KEY (`ID_TIPO_STEP`);

--
-- AUTO_INCREMENT per le tabelle scaricate
--

--
-- AUTO_INCREMENT per la tabella `COATING`
--
ALTER TABLE `COATING`
  MODIFY `ID_COATING` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT per la tabella `COMP_STEP`
--
ALTER TABLE `COMP_STEP`
  MODIFY `ID_COMP_STEP` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=29;
--
-- AUTO_INCREMENT per la tabella `CRN`
--
ALTER TABLE `CRN`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT per la tabella `ETCHING`
--
ALTER TABLE `ETCHING`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT per la tabella `ETCHING_SOFT`
--
ALTER TABLE `ETCHING_SOFT`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT per la tabella `GIALLO`
--
ALTER TABLE `GIALLO`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT per la tabella `GIALLO-SECONDA`
--
ALTER TABLE `GIALLO-SECONDA`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
--
-- AUTO_INCREMENT per la tabella `NERO`
--
ALTER TABLE `NERO`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT per la tabella `NERO-COLORE`
--
ALTER TABLE `NERO-COLORE`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT per la tabella `NERO_INTERFACCIA`
--
ALTER TABLE `NERO_INTERFACCIA`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT per la tabella `PIRONI_CROMO`
--
ALTER TABLE `PIRONI_CROMO`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT per la tabella `PLASMA_ETCHING`
--
ALTER TABLE `PLASMA_ETCHING`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT per la tabella `PLASMA_ETCHING_SECONDA`
--
ALTER TABLE `PLASMA_ETCHING_SECONDA`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT per la tabella `PROCESSO`
--
ALTER TABLE `PROCESSO`
  MODIFY `ID_PROCESSO` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=18;
--
-- AUTO_INCREMENT per la tabella `PULIZIA`
--
ALTER TABLE `PULIZIA`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
--
-- AUTO_INCREMENT per la tabella `ROSSO`
--
ALTER TABLE `ROSSO`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT per la tabella `ROSSO-REPIT`
--
ALTER TABLE `ROSSO-REPIT`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT per la tabella `ROSSO_COLORE`
--
ALTER TABLE `ROSSO_COLORE`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT per la tabella `STEP`
--
ALTER TABLE `STEP`
  MODIFY `ID_STEP` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=187;
--
-- AUTO_INCREMENT per la tabella `TEST_CATODI`
--
ALTER TABLE `TEST_CATODI`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;
--
-- AUTO_INCREMENT per la tabella `TICN_BASSO`
--
ALTER TABLE `TICN_BASSO`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT per la tabella `TICN_COLORE`
--
ALTER TABLE `TICN_COLORE`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT per la tabella `TIN_INTERFACCIA`
--
ALTER TABLE `TIN_INTERFACCIA`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT per la tabella `TIN_PIRONI`
--
ALTER TABLE `TIN_PIRONI`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT per la tabella `TIPO_COMP`
--
ALTER TABLE `TIPO_COMP`
  MODIFY `ID_TIPO_COMP` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=61;
--
-- AUTO_INCREMENT per la tabella `TIPO_STEP`
--
ALTER TABLE `TIPO_STEP`
  MODIFY `ID_TIPO_STEP` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
