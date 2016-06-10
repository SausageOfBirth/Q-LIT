-- SYNTAX

import qualified Control.Exception as E
import Control.Monad
import System.IO
import System.Environment
import Test.HUnit

import QuantumParser
import SyntaxAnalysis
import QSyntax
import CustomError
import DefAnalysis
import CircuitAnalysis

-- FIND TESTS
test11 = TestCase (assertEqual
  "for find gate test,"
  (Right ("g3",6))
  (find_g "test" "g3" [("g1",2),("g2",4),("g3",6)]))
test12 = TestCase (assertEqual
  "for find gate test,"
  (Left $ GateNotDef "" "")
  (find_g "test" "fail" [("g1",2),("g2",4),("g3",6)]))
test13 = TestCase (assertEqual
  "for find qubit test,"
  (Right ("q1","01"))
  (find_q "test" "q1" [("q1","01"),("q2","001"),("q3","000")]))
test14 = TestCase (assertEqual
  "for find qubit test,"
  (Left $ QubitNotDef "" "")
  (find_q "test" "fail" [("q1","01"),("q2","001"),("q3","000")]))
test15 = TestCase (assertEqual
  "for find circuit test,"
  (Right ("c2",2,2,[],"v"))
  (find_c "test" "c2" [("c1",4,4,[],"q"),("c2",2,2,[],"v"),("c3",6,6,[],"x")]))
test16 = TestCase (assertEqual
  "for find circuit test,"
  (Left $ CircuitNotDef "" "")
  (find_c "test" "fail" [("c1",4,4,[],"q"),("c2",2,2,[],"v"),("c3",6,6,[],"x")]))
test17 = TestCase (assertEqual
  "for find wrapper test,"
  (Right ("g",2))
  (find_w find_g "test" "g" [("g",2),("g2",4),("g3",6)]))
test18 = TestCase (assertEqual
  "for find wrapper test,"
  (Left $ GateNotDef "" "")
  (find_w find_g "test" "fail" [("g",2),("g2",4),("g3",6)]))

tests1 = TestList [TestLabel "SuccessFindGate" test11,
                   TestLabel "FailureFindGate" test12,
                   TestLabel "SuccessFindQubit" test13,
                   TestLabel "FailureFindQubit" test14,
                   TestLabel "SuccessFindCircuit" test15,
                   TestLabel "FailureFindCircuit" test16,
                   TestLabel "SucessWrapperFind" test17,
                   TestLabel "FailureWrapperFind" test18
                  ]

-- DEF TESTS
test21 = TestCase (assertEqual
  "for check avail test"
  (Right "avail")
  (check_avail "avail" [("g1",2),("g2",4),("g3",6)]
                       [("q1","01"),("q2","001"),("q3","000")]
                       [("c1",4,4,[],"q"),("c2",2,2,[],"v"),("c3",6,6,[],"x")]
  ))
test22 = TestCase (assertEqual
  "for check avail test"
  (Left $ CircuitExists {})
  (check_avail "c1" [("g1",2),("g2",4),("g3",6)]
                    [("q1","01"),("q2","001"),("q3","000")]
                    [("c1",4,4,[],"q"),("c2",2,2,[],"v"),("c3",6,6,[],"x")]
  ))
test23 = TestCase (assertEqual
  "for check avail test"
  (Left $ GateExists {})
  (check_avail "g2" [("g1",2),("g2",4),("g3",6)]
                    [("q1","01"),("q2","001"),("q3","000")]
                    [("c1",4,4,[],"q"),("c2",2,2,[],"v"),("c3",6,6,[],"x")]
  ))
test24 = TestCase (assertEqual
  "for check avail test"
  (Left $ QubitExists {})
  (check_avail "q3" [("g1",2),("g2",4),("g3",6)]
                    [("q1","01"),("q2","001"),("q3","000")]
                    [("c1",4,4,[],"q"),("c2",2,2,[],"v"),("c3",6,6,[],"x")]
  ))

test25 = TestCase (assertEqual
  "for check unit_mat_m test"
  (Right "g1")
  (unit_mat_m "test" "g1" (Matrix [[Complex 1.0 0.0, Complex 2.0 0.0],
                                   [Complex 3.0 0.0, Complex 4.0 0.0]])
  ))
test26 = TestCase (assertEqual
  "for check unit_mat_m test"
  (Left $ MatNotSquare {})
  (unit_mat_m "test" "g1" (Matrix[[Complex 1.0 0.0, Complex 2.0 0.0,Complex 3.0 0.0],
                                  [Complex 3.0 0.0, Complex 4.0 0.0,Complex 5.0 0.0]])
  ))
test27 = TestCase (assertEqual
  "for check_reg test"
  (Right "g1")
  (check_reg_m "test" "g1" (Matrix[[Complex 1.0 0.0, Complex 2.0 0.0,Complex 3.0 0.0],
                                  [Complex 3.0 0.0, Complex 4.0 0.0,Complex 5.0 0.0]])
  ))
test28 = TestCase (assertEqual
  "for check_reg test"
  (Left $ MatNotReg {})
  (check_reg_m "test" "g1" (Matrix[[Complex 1.0 0.0, Complex 2.0 0.0],
                                  [Complex 3.0 0.0, Complex 4.0 0.0,Complex 5.0 0.0]])
  ))
test29 = TestCase (assertEqual
  "for check_power test"
  (Right "g1")
  (check_power_m "test" "g1" (Matrix[[Complex 1.0 0.0, Complex 2.0 0.0],
                                     [Complex 3.0 0.0, Complex 4.0 0.0]])
  ))
test210 = TestCase (assertEqual
  "for check_power test"
  (Left $ MatNotPower {})
  (check_power_m "test" "g1" (Matrix[[Complex 1.0 0.0]])
  ))
test211 = TestCase (assertEqual
  "for subgate_def test"
  (Right "g1")
  (subgate_def_m "test" "g1" (MBinary Multiply (GateIdf "prev_g") (GateIdf "prev_g2"))
    [("prev_g",4),("prev_g2",6)]
  ))
test212 = TestCase (assertEqual
  "for subgate_def test"
  (Left $ SubGateNotDef {})
  (subgate_def_m "test" "g1"
    (MBinary Multiply (GateIdf "prev_g") (GateIdf "prev_g2")) [("prev_g",2)]
  ))
test213 = TestCase (assertEqual
  "for check_dim test 1"
  (Right "g")
  (check_dim_m "test" "g"
    (MBinary Add (MBinary Kronecker (GateIdf "g3")
    (MBinary Multiply (Ket "01") (Bra "10"))) (GateIdf "g2"))
    [("g2",4),("g3",2)]
  ))
test214 = TestCase (assertEqual
  "for check_dim test 2"
  (Right "g")
  (check_dim_m "test" "g"
    (MBinary Multiply (Matrix [[Complex 0.0 0.0, Complex 0.0 0.0],[Complex 0.0 0.0,
                                Complex 0.0 0.0]]) (IntConst (Complex 1.0 0.0)))
    []
  ))
test215 = TestCase (assertEqual
  "for check_dim test"
  (Left $ GateWrongDim {})
  (check_dim_m "test" "g"
    (MBinary Subtract (Matrix [[Complex 0.0 0.0, Complex 0.0 0.0],[Complex 0.0 0.0,
                               Complex 0.0 0.0]]) (GateIdf "g2"))
    [("g2",4)]
  ))
test216 = TestCase (assertEqual
  "for check exp test"
  (Right "g")
  (check_exp_m "test" "g" (MBinary Exponent (IntConst (Complex 1.0 0.0))
                                            (IntConst (Complex 2.0 0.0)))
  ))
test217 = TestCase (assertEqual
  "for check exp test"
  (Left $ ScalarOnlyExp {})
  (check_exp_m "test" "g" (MBinary Exponent (Bra "01") (IntConst (Complex 2.0 0.0)))
  ))
test218 = TestCase (assertEqual
  "for check exp test"
  (Left $ ScalarOnlyExp {})
  (check_exp_m "test" "g" (MBinary Exponent (IntConst (Complex 1.0 0.0)) (Ket "10"))
  ))

tests2 = TestList [TestLabel "SuccessAvail" test21,
                   TestLabel "FailAvailCirc" test22,
                   TestLabel "FailAvailGate" test23,
                   TestLabel "FailAvailQubit" test24,
                   TestLabel "SuccessUnitMatrixCheck" test25,
                   TestLabel "FailUnitMatrixCheck" test26,
                   TestLabel "SuccessRegMatrixCheck" test27,
                   TestLabel "FailRegMatrixCheck" test28,
                   TestLabel "SuccessPowMatrixCheck" test29,
                   TestLabel "FailPowMatrixCheck" test210,
                   TestLabel "SuccessSubGateDefCheck" test211,
                   TestLabel "FailSubGateDefCheck" test212,
                   TestLabel "SuccessDimCheck1" test213,
                   TestLabel "SuccessDimCheck2" test214,
                   TestLabel "FailDimCheck" test215,
                   TestLabel "SuccessSclarExp" test216,
                   TestLabel "FailScalarCheck1" test217,
                   TestLabel "FailScalarCheck2" test218
                  ]

-- INSERT TESTS
test31 = TestCase (assertEqual
  "for out_of_bounds test"
  (Right "c")
  (out_of_bounds "test" "c" "g" 0 [0] False ("c",4,4,[],""))
  )

test32 = TestCase (assertEqual
  "for out_of_bounds test"
  (Left $ OutOfBounds {})
  (out_of_bounds "test" "c" "g" 0 [6] False ("c",4,4,[],""))
  )

test33 = TestCase (assertEqual
  "for out_of_bounds test"
  (Left $ OutOfBounds {})
  (out_of_bounds "test" "c" "g" 5 [2] False ("c",4,4,[],""))
  )

test34 = TestCase (assertEqual
  "for occupied test"
  (Right "c")
  (occupied "test" "c" "g" 0 [0] False ("c",4,4,[],""))
  )

test35 = TestCase (assertEqual
  "for occupied test"
  (Left $ SlotOccupied {})
  (occupied "test" "c" "g" 0 [0] False ("c",4,4,[(0,0)],""))
  )

test36 = TestCase (assertEqual
  "for correct nr wires test"
  (Right "c")
  (cor_nw "test" "c" "g" [0,1,2] False [("g",8)])
  )

test37 = TestCase (assertEqual
  "for correct nr wires test"
  (Right "c")
  (cor_nw "test" "c" "g" [0,1,2,3] True [("g",2)])
  )

test38 = TestCase (assertEqual
  "for correct nr wires test"
  (Left $ GateNotFit {})
  (cor_nw "test" "c" "g" [0,1,2,3] False [("g",4)])
  )

test39 = TestCase (assertEqual
  "for correct nr wires test"
  (Left $ GateNotFit {})
  (cor_nw "test" "c" "g" [0,1,2,3] True [("g",4)])
  )

test310 = TestCase (assertEqual
  "for adj_wires test"
  (Right "c")
  (adj_wires "test" "c" "g" 0 [0,1,2,3,4] False ("c",5,5,[],""))
  )

test311 = TestCase (assertEqual
  "for adj_wires test"
  (Left $ WiresNotAdj {})
  (adj_wires "test" "c" "g" 0 [0,1,3,4] False ("c",5,5,[],""))
  )

test312 = TestCase (assertEqual
  "for check_cont test"
  (Right "c")
  (check_cont "test" "c" "g" 0 [0,1,2,3] True ("c",5,5,[],""))
  )

test313 = TestCase (assertEqual
  "for check_cont test"
  (Left $ ControlBitsWrong {})
  (check_cont "test" "c" "g" 0 [0] True ("c",5,5,[],""))
  )

tests3 = TestList [TestLabel "SuccessOutOfBounds" test31,
                   TestLabel "FailureOutOfBounds1" test32,
                   TestLabel "FailureOutOfBounds2" test33,
                   TestLabel "SuccessOccupiedTest" test34,
                   TestLabel "FailOccupiedTest" test35,
                   TestLabel "SuccessNrWires" test36,
                   TestLabel "SuccessNrWires2" test37,
                   TestLabel "FailNrWires" test38,
                   TestLabel "FailNrWires2" test39,
                   TestLabel "SuccessAdjacentWires" test310,
                   TestLabel "FailAdjacentWires" test311,
                   TestLabel "SuccessControlBit" test312,
                   TestLabel "FailControlBit" test313
                  ]

-- ATTACH TESTS
test41 = TestCase (assertEqual
  "for qubit_len test"
  (Right "q")
  (qubit_len "test" "q" ("q","0001") ("c",4,4,[],""))
  )

test42 = TestCase (assertEqual
  "for qubit_len test"
  (Left $ BadQubitLength {})
  (qubit_len "test" "q" ("q","00001") ("c",4,4,[],""))
  )

test43 = TestCase (assertEqual
  "for qubit_assign test"
  (Right "q")
  (qubit_assign "test" "q" ("q","0001") ("c",4,4,[],""))
  )

test44 = TestCase (assertEqual
  "for qubit_assign test"
  (Left $ BadQubitAssign {})
  (qubit_assign "test" "q" ("q","0001") ("c",4,4,[],"q2"))
  )

tests4 = TestList [TestLabel "SuccessQubitLength" test41,
                   TestLabel "FailureQubitLength" test42,
                   TestLabel "SuccessQubitAssign" test43,
                   TestLabel "FailureQubitAssign" test44
                  ]

-- DEF ANALYSIS TESTS
prog =
  (Right (Seq [(Def "c" (QCirc 4 4)),
               (Insert "g" "c" 0 [0] False),
               (Def "q" (QBit (Bra "01"))),
               (Def "g" (QGate (GateIdf "g1"))),
               (Attach "q" "c")]
  ))

test51 = TestCase (assertEqual
  "for filter_def test"
  [(Def "c" (QCirc 4 4)),(Def "q" (QBit (Bra "01"))),(Def "g" (QGate (GateIdf "g1")))]
  (filter_def prog)
  )

test52 = TestCase (assertEqual
  "for filter_mat test"
  [("g",(Def "g" (QGate (GateIdf "g1"))))]
  (filter_mat $ filter_def prog)
  )

test53 = TestCase (assertEqual
  "for filter_bit test"
  [("q",Def "q" (QBit (Bra "01")))]
  (filter_bit $ filter_def prog)
  )

test54 = TestCase (assertEqual
  "for filter_dcirc test"
  [("c",Def "c" (QCirc 4 4))]
  (filter_dcirc $ filter_def prog)
  )

mexpr = (MBinary Kronecker (MBinary Multiply (Ket "00") (Bra "00"))
                           (IntConst (Complex 0.0 0.0)))

test55 = TestCase (assertEqual
  "for control test"
  (Comp ("scalarG","matrix")
    (Atom "scalar" (IntConst (Complex 0.0 0.0)))
    (Comp ("outerProduct","matrix") (Atom "ket" (Ket "00")) (Atom "bra" (Bra "00"))))
  (control mexpr)
  )

tests5 = TestList [TestLabel "FilterDefs" test51,
                   TestLabel "FilterMats" test52,
                   TestLabel "FilterBits" test53,
                   TestLabel "FilterCircs" test54,
                   TestLabel "ControlTest" test55
                  ]

-- CIRCUIT ANALYSIS TESTS

test61 = TestCase (assertEqual
  "for filter_insert test"
  [(Insert "g" "c" 0 [0] False)]
  (filter_insert prog)
  )

test62 = TestCase (assertEqual
  "for filter_attach test"
  [("q",Attach "q" "c")]
  (filter_attach prog)
  )

test63 = TestCase (assertEqual
  "for filter_dim test"
  (Just (4,4))
  (filter_dim "c3" [("c2",Def "c2" (QCirc 8 8)),("c",Def "c3" (QCirc 4 4))])
  )

test64 = TestCase (assertEqual
  "for plot_circuit test"
  [("g",8,[0],"c",False),("g",4,[2],"c",False)]
  (plot_circuit [(Insert "c" "g" 8 [0] False),(Insert "c" "g" 4 [2] False)])
  )

test65 = TestCase (assertEqual
  "for plot_identity test"
  [("g",1,[1],"c",False),("g",1,[0],"c",False),
   ("identity",0,[1],"c",False),("identity",0,[0],"c",False)]
  (plot_identity 2 2 [("g",1,[0],"c",False),("g",1,[1],"c",False)])
  )

test66 = TestCase (assertEqual
  "for gate sorting test"
  [("g",1,[1],"c",False),("g",1,[0],"c",False),
   ("identity",0,[1],"c",False),("identity",0,[0],"c",False)]
  (plot_identity 2 2 [("g",1,[1],"c",False),("g",1,[0],"c",False),
                     ("identity",0,[1],"c",False),("identity",0,[0],"c",False)])
  )

tests6 = TestList [TestLabel "FilterInsertions" test61,
                   TestLabel "FilterAttachments" test62,
                   TestLabel "FilterDimensions" test63,
                   TestLabel "PlotCircuit" test64,
                   TestLabel "PlotIdentity" test65,
                   TestLabel "SortCircuit" test66
                  ]

main =
  do
    putStrLn "  -- Syntax analysis tests -- \n"
    putStrLn "  Find tests:"
    runTestTT tests1

    putStrLn "\n  Def tests:"
    runTestTT tests2

    putStrLn "\n Insert tests:"
    runTestTT tests3

    putStrLn "\n Attach tests:"
    runTestTT tests4

    putStrLn "\n  -- Def Analysis tests -- \n"
    runTestTT tests5

    putStrLn "\n  -- Circuit Analysis tests -- \n"
    runTestTT tests6

    --putStrLn "\n -- Tests of larger subparts of system? -- \n"

    return ()

