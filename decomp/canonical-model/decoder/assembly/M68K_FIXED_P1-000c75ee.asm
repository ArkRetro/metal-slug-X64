0C75EE: 0828 0000 0082 btst    #$0, ($82,A0)
0C75F4: 6700 0406      beq     $c79fc
0C75F8: 08A8 0007 0082 bclr    #$7, ($82,A0)
0C75FE: 6700 03FC      beq     $c79fc
0C7602: 3E28 002C      move.w  ($2c,A0), D7
0C7606: 6B00 03F4      bmi     $c79fc
0C760A: 3C07           move.w  D7, D6
0C760C: 0646 8401      addi.w  #-$7bff, D6
0C7610: 0647 8201      addi.w  #-$7dff, D7
0C7614: 4847           swap    D7
0C7616: 4846           swap    D6
0C7618: 3C28 0084      move.w  ($84,A0), D6
0C761C: EF4E           lsl.w   #7, D6
0C761E: 3028 0086      move.w  ($86,A0), D0
0C7622: EF48           lsl.w   #7, D0
0C7624: 43E8 0042      lea     ($42,A0), A1
0C7628: 2A3C 0001 0000 move.l  #$10000, D5
0C762E: 383C 0800      move.w  #$800, D4
0C7632: 363C A000      move.w  #$a000, D3
0C7636: 49F9 003C 0000 lea     $3c0000.l, A4
0C763C: 3E00           move.w  D0, D7
0C763E: 3219           move.w  (A1)+, D1
0C7640: 0646 0400      addi.w  #$400, D6
0C7644: B646           cmp.w   D6, D3
0C7646: 6300 0004      bls     $c764c
0C764A: DE41           add.w   D1, D7
0C764C: 0446 0400      subi.w  #$400, D6
0C7650: 2886           move.l  D6, (A4)
0C7652: DC85           add.l   D5, D6
0C7654: DC44           add.w   D4, D6
0C7656: 2887           move.l  D7, (A4)
0C7658: DE85           add.l   D5, D7
0C765A: 3E00           move.w  D0, D7
0C765C: 3219           move.w  (A1)+, D1
0C765E: 0646 0400      addi.w  #$400, D6
0C7662: B646           cmp.w   D6, D3
0C7664: 6300 0004      bls     $c766a
0C7668: DE41           add.w   D1, D7
0C766A: 0446 0400      subi.w  #$400, D6
0C766E: 2886           move.l  D6, (A4)
0C7670: DC85           add.l   D5, D6
0C7672: DC44           add.w   D4, D6
0C7674: 2887           move.l  D7, (A4)
0C7676: DE85           add.l   D5, D7
0C7678: 3E00           move.w  D0, D7
0C767A: 3219           move.w  (A1)+, D1
0C767C: 0646 0400      addi.w  #$400, D6
0C7680: B646           cmp.w   D6, D3
0C7682: 6300 0004      bls     $c7688
0C7686: DE41           add.w   D1, D7
0C7688: 0446 0400      subi.w  #$400, D6
0C768C: 2886           move.l  D6, (A4)
0C768E: DC85           add.l   D5, D6
0C7690: DC44           add.w   D4, D6
0C7692: 2887           move.l  D7, (A4)
0C7694: DE85           add.l   D5, D7
0C7696: 3E00           move.w  D0, D7
0C7698: 3219           move.w  (A1)+, D1
0C769A: 0646 0400      addi.w  #$400, D6
0C769E: B646           cmp.w   D6, D3
0C76A0: 6300 0004      bls     $c76a6
0C76A4: DE41           add.w   D1, D7
0C76A6: 0446 0400      subi.w  #$400, D6
0C76AA: 2886           move.l  D6, (A4)
0C76AC: DC85           add.l   D5, D6
0C76AE: DC44           add.w   D4, D6
0C76B0: 2887           move.l  D7, (A4)
0C76B2: DE85           add.l   D5, D7
0C76B4: 3E00           move.w  D0, D7
0C76B6: 3219           move.w  (A1)+, D1
0C76B8: 0646 0400      addi.w  #$400, D6
0C76BC: B646           cmp.w   D6, D3
0C76BE: 6300 0004      bls     $c76c4
0C76C2: DE41           add.w   D1, D7
0C76C4: 0446 0400      subi.w  #$400, D6
0C76C8: 2886           move.l  D6, (A4)
0C76CA: DC85           add.l   D5, D6
0C76CC: DC44           add.w   D4, D6
0C76CE: 2887           move.l  D7, (A4)
0C76D0: DE85           add.l   D5, D7
0C76D2: 3E00           move.w  D0, D7
0C76D4: 3219           move.w  (A1)+, D1
0C76D6: 0646 0400      addi.w  #$400, D6
0C76DA: B646           cmp.w   D6, D3
0C76DC: 6300 0004      bls     $c76e2
0C76E0: DE41           add.w   D1, D7
0C76E2: 0446 0400      subi.w  #$400, D6
0C76E6: 2886           move.l  D6, (A4)
0C76E8: DC85           add.l   D5, D6
0C76EA: DC44           add.w   D4, D6
0C76EC: 2887           move.l  D7, (A4)
0C76EE: DE85           add.l   D5, D7
0C76F0: 3E00           move.w  D0, D7
0C76F2: 3219           move.w  (A1)+, D1
0C76F4: 0646 0400      addi.w  #$400, D6
0C76F8: B646           cmp.w   D6, D3
0C76FA: 6300 0004      bls     $c7700
0C76FE: DE41           add.w   D1, D7
0C7700: 0446 0400      subi.w  #$400, D6
0C7704: 2886           move.l  D6, (A4)
0C7706: DC85           add.l   D5, D6
0C7708: DC44           add.w   D4, D6
0C770A: 2887           move.l  D7, (A4)
0C770C: DE85           add.l   D5, D7
0C770E: 3E00           move.w  D0, D7
0C7710: 3219           move.w  (A1)+, D1
0C7712: 0646 0400      addi.w  #$400, D6
0C7716: B646           cmp.w   D6, D3
0C7718: 6300 0004      bls     $c771e
0C771C: DE41           add.w   D1, D7
0C771E: 0446 0400      subi.w  #$400, D6
0C7722: 2886           move.l  D6, (A4)
0C7724: DC85           add.l   D5, D6
0C7726: DC44           add.w   D4, D6
0C7728: 2887           move.l  D7, (A4)
0C772A: DE85           add.l   D5, D7
0C772C: 3E00           move.w  D0, D7
0C772E: 3219           move.w  (A1)+, D1
0C7730: 0646 0400      addi.w  #$400, D6
0C7734: B646           cmp.w   D6, D3
0C7736: 6300 0004      bls     $c773c
0C773A: DE41           add.w   D1, D7
0C773C: 0446 0400      subi.w  #$400, D6
0C7740: 2886           move.l  D6, (A4)
0C7742: DC85           add.l   D5, D6
0C7744: DC44           add.w   D4, D6
0C7746: 2887           move.l  D7, (A4)
0C7748: DE85           add.l   D5, D7
0C774A: 3E00           move.w  D0, D7
0C774C: 3219           move.w  (A1)+, D1
0C774E: 0646 0400      addi.w  #$400, D6
0C7752: B646           cmp.w   D6, D3
0C7754: 6300 0004      bls     $c775a
0C7758: DE41           add.w   D1, D7
0C775A: 0446 0400      subi.w  #$400, D6
0C775E: 2886           move.l  D6, (A4)
0C7760: DC85           add.l   D5, D6
0C7762: DC44           add.w   D4, D6
0C7764: 2887           move.l  D7, (A4)
0C7766: DE85           add.l   D5, D7
0C7768: 3E00           move.w  D0, D7
0C776A: 3219           move.w  (A1)+, D1
0C776C: 0646 0400      addi.w  #$400, D6
0C7770: B646           cmp.w   D6, D3
0C7772: 6300 0004      bls     $c7778
0C7776: DE41           add.w   D1, D7
0C7778: 0446 0400      subi.w  #$400, D6
0C777C: 2886           move.l  D6, (A4)
0C777E: DC85           add.l   D5, D6
0C7780: DC44           add.w   D4, D6
0C7782: 2887           move.l  D7, (A4)
0C7784: DE85           add.l   D5, D7
0C7786: 3E00           move.w  D0, D7
0C7788: 3219           move.w  (A1)+, D1
0C778A: 0646 0400      addi.w  #$400, D6
0C778E: B646           cmp.w   D6, D3
0C7790: 6300 0004      bls     $c7796
0C7794: DE41           add.w   D1, D7
0C7796: 0446 0400      subi.w  #$400, D6
0C779A: 2886           move.l  D6, (A4)
0C779C: DC85           add.l   D5, D6
0C779E: DC44           add.w   D4, D6
0C77A0: 2887           move.l  D7, (A4)
0C77A2: DE85           add.l   D5, D7
0C77A4: 3E00           move.w  D0, D7
0C77A6: 3219           move.w  (A1)+, D1
0C77A8: 0646 0400      addi.w  #$400, D6
0C77AC: B646           cmp.w   D6, D3
0C77AE: 6300 0004      bls     $c77b4
0C77B2: DE41           add.w   D1, D7
0C77B4: 0446 0400      subi.w  #$400, D6
0C77B8: 2886           move.l  D6, (A4)
0C77BA: DC85           add.l   D5, D6
0C77BC: DC44           add.w   D4, D6
0C77BE: 2887           move.l  D7, (A4)
0C77C0: DE85           add.l   D5, D7
0C77C2: 3E00           move.w  D0, D7
0C77C4: 3219           move.w  (A1)+, D1
0C77C6: 0646 0400      addi.w  #$400, D6
0C77CA: B646           cmp.w   D6, D3
0C77CC: 6300 0004      bls     $c77d2
0C77D0: DE41           add.w   D1, D7
0C77D2: 0446 0400      subi.w  #$400, D6
0C77D6: 2886           move.l  D6, (A4)
0C77D8: DC85           add.l   D5, D6
0C77DA: DC44           add.w   D4, D6
0C77DC: 2887           move.l  D7, (A4)
0C77DE: DE85           add.l   D5, D7
0C77E0: 3E00           move.w  D0, D7
0C77E2: 3219           move.w  (A1)+, D1
0C77E4: 0646 0400      addi.w  #$400, D6
0C77E8: B646           cmp.w   D6, D3
0C77EA: 6300 0004      bls     $c77f0
0C77EE: DE41           add.w   D1, D7
0C77F0: 0446 0400      subi.w  #$400, D6
0C77F4: 2886           move.l  D6, (A4)
0C77F6: DC85           add.l   D5, D6
0C77F8: DC44           add.w   D4, D6
0C77FA: 2887           move.l  D7, (A4)
0C77FC: DE85           add.l   D5, D7
0C77FE: 3E00           move.w  D0, D7
0C7800: 3219           move.w  (A1)+, D1
0C7802: 0646 0400      addi.w  #$400, D6
0C7806: B646           cmp.w   D6, D3
0C7808: 6300 0004      bls     $c780e
0C780C: DE41           add.w   D1, D7
0C780E: 0446 0400      subi.w  #$400, D6
0C7812: 2886           move.l  D6, (A4)
0C7814: DC85           add.l   D5, D6
0C7816: DC44           add.w   D4, D6
0C7818: 2887           move.l  D7, (A4)
0C781A: DE85           add.l   D5, D7
0C781C: 3E00           move.w  D0, D7
0C781E: 3219           move.w  (A1)+, D1
0C7820: 0646 0400      addi.w  #$400, D6
0C7824: B646           cmp.w   D6, D3
0C7826: 6300 0004      bls     $c782c
0C782A: DE41           add.w   D1, D7
0C782C: 0446 0400      subi.w  #$400, D6
0C7830: 2886           move.l  D6, (A4)
0C7832: DC85           add.l   D5, D6
0C7834: DC44           add.w   D4, D6
0C7836: 2887           move.l  D7, (A4)
0C7838: DE85           add.l   D5, D7
0C783A: 3E00           move.w  D0, D7
0C783C: 3219           move.w  (A1)+, D1
0C783E: 0646 0400      addi.w  #$400, D6
0C7842: B646           cmp.w   D6, D3
0C7844: 6300 0004      bls     $c784a
0C7848: DE41           add.w   D1, D7
0C784A: 0446 0400      subi.w  #$400, D6
0C784E: 2886           move.l  D6, (A4)
0C7850: DC85           add.l   D5, D6
0C7852: DC44           add.w   D4, D6
0C7854: 2887           move.l  D7, (A4)
0C7856: DE85           add.l   D5, D7
0C7858: 3E00           move.w  D0, D7
0C785A: 3219           move.w  (A1)+, D1
0C785C: 0646 0400      addi.w  #$400, D6
0C7860: B646           cmp.w   D6, D3
0C7862: 6300 0004      bls     $c7868
0C7866: DE41           add.w   D1, D7
0C7868: 0446 0400      subi.w  #$400, D6
0C786C: 2886           move.l  D6, (A4)
0C786E: DC85           add.l   D5, D6
0C7870: DC44           add.w   D4, D6
0C7872: 2887           move.l  D7, (A4)
0C7874: DE85           add.l   D5, D7
0C7876: 3E00           move.w  D0, D7
0C7878: 3219           move.w  (A1)+, D1
0C787A: 0646 0400      addi.w  #$400, D6
0C787E: B646           cmp.w   D6, D3
0C7880: 6300 0004      bls     $c7886
0C7884: DE41           add.w   D1, D7
0C7886: 0446 0400      subi.w  #$400, D6
0C788A: 2886           move.l  D6, (A4)
0C788C: DC85           add.l   D5, D6
0C788E: DC44           add.w   D4, D6
0C7890: 2887           move.l  D7, (A4)
0C7892: DE85           add.l   D5, D7
0C7894: 3E00           move.w  D0, D7
0C7896: 3219           move.w  (A1)+, D1
0C7898: 0646 0400      addi.w  #$400, D6
0C789C: B646           cmp.w   D6, D3
0C789E: 6300 0004      bls     $c78a4
0C78A2: DE41           add.w   D1, D7
0C78A4: 0446 0400      subi.w  #$400, D6
0C78A8: 2886           move.l  D6, (A4)
0C78AA: DC85           add.l   D5, D6
0C78AC: DC44           add.w   D4, D6
0C78AE: 2887           move.l  D7, (A4)
0C78B0: DE85           add.l   D5, D7
0C78B2: 3E00           move.w  D0, D7
0C78B4: 3219           move.w  (A1)+, D1
0C78B6: 0646 0400      addi.w  #$400, D6
0C78BA: B646           cmp.w   D6, D3
0C78BC: 6300 0004      bls     $c78c2
0C78C0: DE41           add.w   D1, D7
0C78C2: 0446 0400      subi.w  #$400, D6
0C78C6: 2886           move.l  D6, (A4)
0C78C8: DC85           add.l   D5, D6
0C78CA: DC44           add.w   D4, D6
0C78CC: 2887           move.l  D7, (A4)
0C78CE: DE85           add.l   D5, D7
0C78D0: 3E00           move.w  D0, D7
0C78D2: 3219           move.w  (A1)+, D1
0C78D4: 0646 0400      addi.w  #$400, D6
0C78D8: B646           cmp.w   D6, D3
0C78DA: 6300 0004      bls     $c78e0
0C78DE: DE41           add.w   D1, D7
0C78E0: 0446 0400      subi.w  #$400, D6
0C78E4: 2886           move.l  D6, (A4)
0C78E6: DC85           add.l   D5, D6
0C78E8: DC44           add.w   D4, D6
0C78EA: 2887           move.l  D7, (A4)
0C78EC: DE85           add.l   D5, D7
0C78EE: 3E00           move.w  D0, D7
0C78F0: 3219           move.w  (A1)+, D1
0C78F2: 0646 0400      addi.w  #$400, D6
0C78F6: B646           cmp.w   D6, D3
0C78F8: 6300 0004      bls     $c78fe
0C78FC: DE41           add.w   D1, D7
0C78FE: 0446 0400      subi.w  #$400, D6
0C7902: 2886           move.l  D6, (A4)
0C7904: DC85           add.l   D5, D6
0C7906: DC44           add.w   D4, D6
0C7908: 2887           move.l  D7, (A4)
0C790A: DE85           add.l   D5, D7
0C790C: 3E00           move.w  D0, D7
0C790E: 3219           move.w  (A1)+, D1
0C7910: 0646 0400      addi.w  #$400, D6
0C7914: B646           cmp.w   D6, D3
0C7916: 6300 0004      bls     $c791c
0C791A: DE41           add.w   D1, D7
0C791C: 0446 0400      subi.w  #$400, D6
0C7920: 2886           move.l  D6, (A4)
0C7922: DC85           add.l   D5, D6
0C7924: DC44           add.w   D4, D6
0C7926: 2887           move.l  D7, (A4)
0C7928: DE85           add.l   D5, D7
0C792A: 3E00           move.w  D0, D7
0C792C: 3219           move.w  (A1)+, D1
0C792E: 0646 0400      addi.w  #$400, D6
0C7932: B646           cmp.w   D6, D3
0C7934: 6300 0004      bls     $c793a
0C7938: DE41           add.w   D1, D7
0C793A: 0446 0400      subi.w  #$400, D6
0C793E: 2886           move.l  D6, (A4)
0C7940: DC85           add.l   D5, D6
0C7942: DC44           add.w   D4, D6
0C7944: 2887           move.l  D7, (A4)
0C7946: DE85           add.l   D5, D7
0C7948: 3E00           move.w  D0, D7
0C794A: 3219           move.w  (A1)+, D1
0C794C: 0646 0400      addi.w  #$400, D6
0C7950: B646           cmp.w   D6, D3
0C7952: 6300 0004      bls     $c7958
0C7956: DE41           add.w   D1, D7
0C7958: 0446 0400      subi.w  #$400, D6
0C795C: 2886           move.l  D6, (A4)
0C795E: DC85           add.l   D5, D6
0C7960: DC44           add.w   D4, D6
0C7962: 2887           move.l  D7, (A4)
0C7964: DE85           add.l   D5, D7
0C7966: 3E00           move.w  D0, D7
0C7968: 3219           move.w  (A1)+, D1
0C796A: 0646 0400      addi.w  #$400, D6
0C796E: B646           cmp.w   D6, D3
0C7970: 6300 0004      bls     $c7976
0C7974: DE41           add.w   D1, D7
0C7976: 0446 0400      subi.w  #$400, D6
0C797A: 2886           move.l  D6, (A4)
0C797C: DC85           add.l   D5, D6
0C797E: DC44           add.w   D4, D6
0C7980: 2887           move.l  D7, (A4)
0C7982: DE85           add.l   D5, D7
0C7984: 3E00           move.w  D0, D7
0C7986: 3219           move.w  (A1)+, D1
0C7988: 0646 0400      addi.w  #$400, D6
0C798C: B646           cmp.w   D6, D3
0C798E: 6300 0004      bls     $c7994
0C7992: DE41           add.w   D1, D7
0C7994: 0446 0400      subi.w  #$400, D6
0C7998: 2886           move.l  D6, (A4)
0C799A: DC85           add.l   D5, D6
0C799C: DC44           add.w   D4, D6
0C799E: 2887           move.l  D7, (A4)
0C79A0: DE85           add.l   D5, D7
0C79A2: 3E00           move.w  D0, D7
0C79A4: 3219           move.w  (A1)+, D1
0C79A6: 0646 0400      addi.w  #$400, D6
0C79AA: B646           cmp.w   D6, D3
0C79AC: 6300 0004      bls     $c79b2
0C79B0: DE41           add.w   D1, D7
0C79B2: 0446 0400      subi.w  #$400, D6
0C79B6: 2886           move.l  D6, (A4)
0C79B8: DC85           add.l   D5, D6
0C79BA: DC44           add.w   D4, D6
0C79BC: 2887           move.l  D7, (A4)
0C79BE: DE85           add.l   D5, D7
0C79C0: 3E00           move.w  D0, D7
0C79C2: 3219           move.w  (A1)+, D1
0C79C4: 0646 0400      addi.w  #$400, D6
0C79C8: B646           cmp.w   D6, D3
0C79CA: 6300 0004      bls     $c79d0
0C79CE: DE41           add.w   D1, D7
0C79D0: 0446 0400      subi.w  #$400, D6
0C79D4: 2886           move.l  D6, (A4)
0C79D6: DC85           add.l   D5, D6
0C79D8: DC44           add.w   D4, D6
0C79DA: 2887           move.l  D7, (A4)
0C79DC: DE85           add.l   D5, D7
0C79DE: 3E00           move.w  D0, D7
0C79E0: 3219           move.w  (A1)+, D1
0C79E2: 0646 0400      addi.w  #$400, D6
0C79E6: B646           cmp.w   D6, D3
0C79E8: 6300 0004      bls     $c79ee
0C79EC: DE41           add.w   D1, D7
0C79EE: 0446 0400      subi.w  #$400, D6
0C79F2: 2886           move.l  D6, (A4)
0C79F4: DC85           add.l   D5, D6
0C79F6: DC44           add.w   D4, D6
0C79F8: 2887           move.l  D7, (A4)
0C79FA: DE85           add.l   D5, D7
0C79FC: 4E75           rts
