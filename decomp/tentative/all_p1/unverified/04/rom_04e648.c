/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04e648.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x0004e58e) */
/* WARNING: Removing unreachable block (ram,0x0004e59c) */

undefined8 candidate_4e648(void)

{
  uint uVar1;
  int in_D0;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  uint uVar28;
  uint uVar29;
  uint uVar30;
  uint uVar31;
  uint uVar32;
  uint uVar33;
  uint uVar34;
  uint uVar35;
  uint uVar36;
  uint uVar37;
  uint uVar38;
  uint uVar39;
  uint uVar40;
  uint uVar41;
  uint uVar42;
  uint uVar43;
  uint uVar44;
  uint uVar45;
  uint uVar46;
  uint uVar47;
  uint uVar48;
  uint uVar49;
  undefined2 uVar50;
  uint in_D1;
  uint uVar51;
  uint uVar52;

  uVar51 = in_D1;
  if (in_D0 != 0) {
    if (in_D1 == 0) {
      __m68k_trap(0xf);
      return 0;
    }
    uVar2 = in_D0 * 0x10000;
    uVar3 = in_D0 * 0x20000;
    uVar1 = (uint)in_D0 >> 0x10;
    uVar50 = (undefined2)in_D0;
    uVar51 = (uint)CARRY4(uVar2,uVar2);
    if (in_D1 <= uVar51) {
      uVar3 = CONCAT22((short)(uVar3 >> 0x10),1);
      uVar51 = uVar51 - in_D1;
    }
    uVar4 = uVar3 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar3,uVar3);
    if (in_D1 <= uVar51) {
      uVar4 = CONCAT22((short)(uVar4 >> 0x10),(short)uVar4 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar5 = uVar4 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar4,uVar4);
    if (in_D1 <= uVar51) {
      uVar5 = CONCAT22((short)(uVar5 >> 0x10),(short)uVar5 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar6 = uVar5 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar5,uVar5);
    if (in_D1 <= uVar51) {
      uVar6 = CONCAT22((short)(uVar6 >> 0x10),(short)uVar6 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar7 = uVar6 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar6,uVar6);
    if (in_D1 <= uVar51) {
      uVar7 = CONCAT22((short)(uVar7 >> 0x10),(short)uVar7 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar8 = uVar7 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar7,uVar7);
    if (in_D1 <= uVar51) {
      uVar8 = CONCAT22((short)(uVar8 >> 0x10),(short)uVar8 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar9 = uVar8 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar8,uVar8);
    if (in_D1 <= uVar51) {
      uVar9 = CONCAT22((short)(uVar9 >> 0x10),(short)uVar9 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar10 = uVar9 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar9,uVar9);
    if (in_D1 <= uVar51) {
      uVar10 = CONCAT22((short)(uVar10 >> 0x10),(short)uVar10 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar11 = uVar10 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar10,uVar10);
    if (in_D1 <= uVar51) {
      uVar11 = CONCAT22((short)(uVar11 >> 0x10),(short)uVar11 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar12 = uVar11 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar11,uVar11);
    if (in_D1 <= uVar51) {
      uVar12 = CONCAT22((short)(uVar12 >> 0x10),(short)uVar12 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar13 = uVar12 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar12,uVar12);
    if (in_D1 <= uVar51) {
      uVar13 = CONCAT22((short)(uVar13 >> 0x10),(short)uVar13 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar14 = uVar13 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar13,uVar13);
    if (in_D1 <= uVar51) {
      uVar14 = CONCAT22((short)(uVar14 >> 0x10),(short)uVar14 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar15 = uVar14 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar14,uVar14);
    if (in_D1 <= uVar51) {
      uVar15 = CONCAT22((short)(uVar15 >> 0x10),(short)uVar15 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar16 = uVar15 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar15,uVar15);
    if (in_D1 <= uVar51) {
      uVar16 = CONCAT22((short)(uVar16 >> 0x10),(short)uVar16 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar17 = uVar16 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar16,uVar16);
    if (in_D1 <= uVar51) {
      uVar17 = CONCAT22((short)(uVar17 >> 0x10),(short)uVar17 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar18 = uVar17 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar17,uVar17);
    if (in_D1 <= uVar51) {
      uVar18 = CONCAT22((short)(uVar18 >> 0x10),(short)uVar18 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar19 = uVar18 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar18,uVar18);
    if (in_D1 <= uVar51) {
      uVar19 = CONCAT22((short)(uVar19 >> 0x10),(short)uVar19 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar20 = uVar19 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar19,uVar19);
    if (in_D1 <= uVar51) {
      uVar20 = CONCAT22((short)(uVar20 >> 0x10),(short)uVar20 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar21 = uVar20 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar20,uVar20);
    if (in_D1 <= uVar51) {
      uVar21 = CONCAT22((short)(uVar21 >> 0x10),(short)uVar21 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar22 = uVar21 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar21,uVar21);
    if (in_D1 <= uVar51) {
      uVar22 = CONCAT22((short)(uVar22 >> 0x10),(short)uVar22 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar23 = uVar22 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar22,uVar22);
    if (in_D1 <= uVar51) {
      uVar23 = CONCAT22((short)(uVar23 >> 0x10),(short)uVar23 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar24 = uVar23 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar23,uVar23);
    if (in_D1 <= uVar51) {
      uVar24 = CONCAT22((short)(uVar24 >> 0x10),(short)uVar24 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar25 = uVar24 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar24,uVar24);
    if (in_D1 <= uVar51) {
      uVar25 = CONCAT22((short)(uVar25 >> 0x10),(short)uVar25 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar26 = uVar25 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar25,uVar25);
    if (in_D1 <= uVar51) {
      uVar26 = CONCAT22((short)(uVar26 >> 0x10),(short)uVar26 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar27 = uVar26 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar26,uVar26);
    if (in_D1 <= uVar51) {
      uVar27 = CONCAT22((short)(uVar27 >> 0x10),(short)uVar27 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar28 = uVar27 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar27,uVar27);
    if (in_D1 <= uVar51) {
      uVar28 = CONCAT22((short)(uVar28 >> 0x10),(short)uVar28 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar29 = uVar28 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar28,uVar28);
    if (in_D1 <= uVar51) {
      uVar29 = CONCAT22((short)(uVar29 >> 0x10),(short)uVar29 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar30 = uVar29 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar29,uVar29);
    if (in_D1 <= uVar51) {
      uVar30 = CONCAT22((short)(uVar30 >> 0x10),(short)uVar30 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar31 = uVar30 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar30,uVar30);
    if (in_D1 <= uVar51) {
      uVar31 = CONCAT22((short)(uVar31 >> 0x10),(short)uVar31 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar32 = uVar31 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar31,uVar31);
    if (in_D1 <= uVar51) {
      uVar32 = CONCAT22((short)(uVar32 >> 0x10),(short)uVar32 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar33 = uVar32 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar32,uVar32);
    if (in_D1 <= uVar51) {
      uVar33 = CONCAT22((short)(uVar33 >> 0x10),(short)uVar33 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar34 = uVar33 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar33,uVar33);
    if (in_D1 <= uVar51) {
      uVar34 = CONCAT22((short)(uVar34 >> 0x10),(short)uVar34 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar35 = uVar34 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar34,uVar34);
    if (in_D1 <= uVar51) {
      uVar35 = CONCAT22((short)(uVar35 >> 0x10),(short)uVar35 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar36 = uVar35 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar35,uVar35);
    if (in_D1 <= uVar51) {
      uVar36 = CONCAT22((short)(uVar36 >> 0x10),(short)uVar36 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar37 = uVar36 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar36,uVar36);
    if (in_D1 <= uVar51) {
      uVar37 = CONCAT22((short)(uVar37 >> 0x10),(short)uVar37 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar38 = uVar37 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar37,uVar37);
    if (in_D1 <= uVar51) {
      uVar38 = CONCAT22((short)(uVar38 >> 0x10),(short)uVar38 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar39 = uVar38 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar38,uVar38);
    if (in_D1 <= uVar51) {
      uVar39 = CONCAT22((short)(uVar39 >> 0x10),(short)uVar39 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar40 = uVar39 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar39,uVar39);
    if (in_D1 <= uVar51) {
      uVar40 = CONCAT22((short)(uVar40 >> 0x10),(short)uVar40 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar41 = uVar40 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar40,uVar40);
    if (in_D1 <= uVar51) {
      uVar41 = CONCAT22((short)(uVar41 >> 0x10),(short)uVar41 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar42 = uVar41 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar41,uVar41);
    if (in_D1 <= uVar51) {
      uVar42 = CONCAT22((short)(uVar42 >> 0x10),(short)uVar42 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar43 = uVar42 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar42,uVar42);
    if (in_D1 <= uVar51) {
      uVar43 = CONCAT22((short)(uVar43 >> 0x10),(short)uVar43 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar44 = uVar43 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar43,uVar43);
    if (in_D1 <= uVar51) {
      uVar44 = CONCAT22((short)(uVar44 >> 0x10),(short)uVar44 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar45 = uVar44 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar44,uVar44);
    if (in_D1 <= uVar51) {
      uVar45 = CONCAT22((short)(uVar45 >> 0x10),(short)uVar45 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar46 = uVar45 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar45,uVar45);
    if (in_D1 <= uVar51) {
      uVar46 = CONCAT22((short)(uVar46 >> 0x10),(short)uVar46 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar47 = uVar46 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar46,uVar46);
    if (in_D1 <= uVar51) {
      uVar47 = CONCAT22((short)(uVar47 >> 0x10),(short)uVar47 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar48 = uVar47 * 2;
    uVar51 = uVar51 * 2 + (uint)CARRY4(uVar47,uVar47);
    if (in_D1 <= uVar51) {
      uVar48 = CONCAT22((short)(uVar48 >> 0x10),(short)uVar48 + 1);
      uVar51 = uVar51 - in_D1;
    }
    uVar49 = uVar48 * 2;
    uVar52 = uVar51 * 2 + (uint)CARRY4(uVar48,uVar48);
    if (in_D1 <= uVar52) {
      uVar49 = CONCAT22((short)(uVar49 >> 0x10),(short)uVar49 + 1);
      uVar52 = uVar52 - in_D1;
    }
    in_D0 = uVar49 * 2;
    uVar51 = CONCAT22(uVar50,((((((((((((((((((((((((((((((((((((((((((((((((short)uVar1 * 2 +
                                                                           (ushort)CARRY4(uVar2,
                                                  uVar2)) * 2 + (ushort)CARRY4(uVar3,uVar3)) * 2 +
                                                  (ushort)CARRY4(uVar4,uVar4)) * 2 +
                                                  (ushort)CARRY4(uVar5,uVar5)) * 2 +
                                                  (ushort)CARRY4(uVar6,uVar6)) * 2 +
                                                  (ushort)CARRY4(uVar7,uVar7)) * 2 +
                                                  (ushort)CARRY4(uVar8,uVar8)) * 2 +
                                                  (ushort)CARRY4(uVar9,uVar9)) * 2 +
                                                  (ushort)CARRY4(uVar10,uVar10)) * 2 +
                                                  (ushort)CARRY4(uVar11,uVar11)) * 2 +
                                                  (ushort)CARRY4(uVar12,uVar12)) * 2 +
                                                  (ushort)CARRY4(uVar13,uVar13)) * 2 +
                                                  (ushort)CARRY4(uVar14,uVar14)) * 2 +
                                                  (ushort)CARRY4(uVar15,uVar15)) * 2 +
                                                  (ushort)CARRY4(uVar16,uVar16)) * 2 +
                                                  (ushort)CARRY4(uVar17,uVar17)) * 2 +
                                                  (ushort)CARRY4(uVar18,uVar18)) * 2 +
                                                  (ushort)CARRY4(uVar19,uVar19)) * 2 +
                                                  (ushort)CARRY4(uVar20,uVar20)) * 2 +
                                                  (ushort)CARRY4(uVar21,uVar21)) * 2 +
                                                  (ushort)CARRY4(uVar22,uVar22)) * 2 +
                                                  (ushort)CARRY4(uVar23,uVar23)) * 2 +
                                                  (ushort)CARRY4(uVar24,uVar24)) * 2 +
                                                  (ushort)CARRY4(uVar25,uVar25)) * 2 +
                                                  (ushort)CARRY4(uVar26,uVar26)) * 2 +
                                                  (ushort)CARRY4(uVar27,uVar27)) * 2 +
                                                 (ushort)CARRY4(uVar28,uVar28)) * 2 +
                                                (ushort)CARRY4(uVar29,uVar29)) * 2 +
                                               (ushort)CARRY4(uVar30,uVar30)) * 2 +
                                              (ushort)CARRY4(uVar31,uVar31)) * 2 +
                                             (ushort)CARRY4(uVar32,uVar32)) * 2 +
                                            (ushort)CARRY4(uVar33,uVar33)) * 2 +
                                           (ushort)CARRY4(uVar34,uVar34)) * 2 +
                                          (ushort)CARRY4(uVar35,uVar35)) * 2 +
                                         (ushort)CARRY4(uVar36,uVar36)) * 2 +
                                        (ushort)CARRY4(uVar37,uVar37)) * 2 +
                                       (ushort)CARRY4(uVar38,uVar38)) * 2 +
                                      (ushort)CARRY4(uVar39,uVar39)) * 2 +
                                     (ushort)CARRY4(uVar40,uVar40)) * 2 +
                                    (ushort)CARRY4(uVar41,uVar41)) * 2 +
                                   (ushort)CARRY4(uVar42,uVar42)) * 2 +
                                  (ushort)CARRY4(uVar43,uVar43)) * 2 + (ushort)CARRY4(uVar44,uVar44)
                                 ) * 2 + (ushort)CARRY4(uVar45,uVar45)) * 2 +
                               (ushort)CARRY4(uVar46,uVar46)) * 2 + (ushort)CARRY4(uVar47,uVar47)) *
                              2 + (ushort)CARRY4(uVar48,uVar48)) * 2 + (ushort)CARRY4(uVar49,uVar49)
                     );
    if (in_D1 <= uVar52 * 2 + (uint)CARRY4(uVar49,uVar49)) {
      in_D0 = CONCAT22((short)((uint)in_D0 >> 0x10),(short)in_D0 + 1);
    }
  }
  return CONCAT44(in_D0,uVar51);
}
