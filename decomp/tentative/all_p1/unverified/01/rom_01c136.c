/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01c136.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_1c136(void)

{
  code *pcVar1;
  short *psVar2;
  short sVar3;
  ushort uVar4;
  short sVar5;
  ushort uVar6;
  short sVar7;
  short sVar8;
  uint uVar9;
  uint uVar10;
  uint *puVar11;
  uint *puVar12;
  int *piVar13;
  int iVar15;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar16;
  int *piVar14;

  piVar13 = (int *)unaff_A6[0x20];
  if (*piVar13 == 0) {
    candidate_c0d48();
    candidate_4126();
    iVar15 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar15 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  psVar2 = (short *)*piVar13;
  unaff_A6[0x20] = piVar13 + 1;
  if ((DAT_0010f000 & 0x20) != 0) {
    candidate_4dca();
  }
  iVar15 = unaff_A5 + -0x1c1c;
  if (-1 < *(char *)(unaff_A5 + -0x1c1e)) {
    iVar15 = unaff_A5 + -0x1a6a;
  }
  *(undefined2 *)(unaff_A5 + -0x18ac) = 0;
  *(undefined4 *)(unaff_A5 + -0x182a) = 0;
  *(undefined2 *)(unaff_A5 + -0x1826) = 0;
  *(undefined4 *)(unaff_A5 + -0x1824) = 0;
  *(undefined2 *)(unaff_A5 + -0x1820) = 0xffff;
  sVar3 = *psVar2;
  sVar5 = psVar2[1];
  sVar7 = psVar2[2];
  sVar8 = psVar2[3];
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    sVar3 = -(sVar7 + sVar3);
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 2) != 0) {
    sVar5 = -(sVar8 + sVar5);
  }
  uVar4 = *(short *)(unaff_A6 + 0xf) + sVar3 + 0x20;
  if ((short)(*(short *)(unaff_A6 + 0xf) + sVar3) < -0x20) {
    bVar16 = SCARRY2(uVar4,sVar7);
    sVar7 = uVar4 + sVar7;
    if (bVar16 != sVar7 < 0) {
      return 0;
    }
    uVar4 = 0;
  }
  if ((short)uVar4 < 0x180) {
    sVar7 = uVar4 + sVar7;
    if (0x17f < sVar7) {
      sVar7 = 0x17f;
    }
    if ((ushort)(sVar7 - uVar4) != 0) {
      uVar6 = (*(short *)(unaff_A6 + 0x10) + sVar5) - 0x108;
      if ((short)(*(short *)(unaff_A6 + 0x10) + sVar5) < 0x108) {
        bVar16 = SCARRY2(uVar6,sVar8);
        sVar8 = uVar6 + sVar8;
        if (bVar16 != sVar8 < 0) {
          return 0;
        }
        uVar6 = 0;
      }
      if ((short)uVar6 < 0xf0) {
        sVar8 = uVar6 + sVar8;
        if (0xef < sVar8) {
          sVar8 = 0xef;
        }
        if ((ushort)(sVar8 - uVar6) != 0) {
          sVar7 = ((ushort)(uVar4 + (sVar7 - uVar4)) >> 5) - (uVar4 >> 5);
          sVar8 = ((ushort)(uVar6 + (sVar8 - uVar6)) >> 5) - (uVar6 >> 5);
          puVar12 = (uint *)(iVar15 + 0x102 + (int)(short)((uVar4 >> 5) << 3));
          uVar10 = 0;
          uVar9 = 0;
          do {
            puVar11 = puVar12 + 1;
            uVar10 = *puVar12 | uVar10;
            puVar12 = puVar12 + 2;
            uVar9 = *puVar11 | uVar9;
            sVar7 = sVar7 + -1;
          } while (sVar7 != -1);
          *(uint *)(unaff_A5 + -0x18b8) = uVar9;
          *(uint *)(unaff_A5 + -0x18b4) = uVar10;
          if (uVar9 != 0 || uVar10 != 0) {
            puVar12 = (uint *)(iVar15 + 0x162 + (int)(short)((uVar6 >> 5) << 3));
            uVar10 = 0;
            do {
              puVar11 = puVar12 + 1;
              uVar10 = *puVar12 | uVar10;
              puVar12 = puVar12 + 2;
              uVar9 = *puVar11 | uVar9;
              sVar8 = sVar8 + -1;
            } while (sVar8 != -1);
            *(uint *)(unaff_A5 + -0x18b8) = uVar9 & *(uint *)(unaff_A5 + -0x18b8);
            *(uint *)(unaff_A5 + -0x18b4) = uVar10 & *(uint *)(unaff_A5 + -0x18b4);
            if (uVar9 != 0 || uVar10 != 0) {
              piVar13 = (int *)(psVar2 + 5);
              while( true ) {
                piVar14 = piVar13 + 1;
                if (*piVar13 < 0) break;
                if ((short)((uint)*piVar13 >> 0x10) == 0) {
                  puVar12 = (uint *)(iVar15 + 0x1a2);
                }
                else {
                  puVar12 = (uint *)(iVar15 + 0x1aa);
                }
                uVar10 = *puVar12 & *(uint *)(unaff_A5 + -0x18b4);
                uVar9 = puVar12[1] & *(uint *)(unaff_A5 + -0x18b8);
                if (uVar10 != 0) {
                  sVar7 = (short)uVar10;
                  while (uVar4 = (ushort)(uVar10 >> 0x10), sVar7 != 0) {
                    if (sVar7 < 0) {
                      candidate_4762();
                    }
                    sVar7 = (short)uVar10 * 2;
                    uVar10 = CONCAT22(uVar4,sVar7);
                  }
                  uVar10 = uVar10 >> 0x10;
                  while (uVar4 != 0) {
                    if ((short)uVar4 < 0) {
                      candidate_4762();
                    }
                    uVar4 = (short)uVar10 * 2;
                    uVar10 = (uint)uVar4;
                  }
                }
                piVar13 = piVar14;
                if (uVar9 != 0) {
                  sVar7 = (short)uVar9;
                  while (uVar4 = (ushort)(uVar9 >> 0x10), sVar7 != 0) {
                    if (sVar7 < 0) {
                      candidate_4762();
                    }
                    sVar7 = (short)uVar9 * 2;
                    uVar9 = CONCAT22(uVar4,sVar7);
                  }
                  uVar9 = uVar9 >> 0x10;
                  while (uVar4 != 0) {
                    if ((short)uVar4 < 0) {
                      candidate_4762();
                    }
                    uVar4 = (short)uVar9 * 2;
                    uVar9 = (uint)uVar4;
                  }
                }
              }
            }
          }
          return *(undefined2 *)(unaff_A5 + -0x18ac);
        }
      }
    }
  }
  return 0;
}
