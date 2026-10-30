/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0049e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_49e8(void)

{
  ushort uVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  uint uVar7;
  uint uVar8;
  short *in_A0;
  uint *puVar9;
  uint *puVar10;
  int *piVar11;
  int iVar13;
  int unaff_A5;
  int unaff_A6;
  bool bVar14;
  int *piVar12;

  if ((DAT_0010f000 & 0x20) != 0) {
    candidate_4dca();
  }
  iVar13 = unaff_A5 + -0x1c1c;
  if (-1 < *(char *)(unaff_A5 + -0x1c1e)) {
    iVar13 = unaff_A5 + -0x1a6a;
  }
  *(undefined2 *)(unaff_A5 + -0x18ac) = 0;
  *(undefined4 *)(unaff_A5 + -0x182a) = 0;
  *(undefined2 *)(unaff_A5 + -0x1826) = 0;
  *(undefined4 *)(unaff_A5 + -0x1824) = 0;
  *(undefined2 *)(unaff_A5 + -0x1820) = 0xffff;
  sVar4 = *in_A0;
  sVar6 = in_A0[1];
  sVar3 = in_A0[2];
  sVar5 = in_A0[3];
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar4 = -(sVar3 + sVar4);
  }
  sVar4 = *(short *)(unaff_A6 + 0x3c) + sVar4;
  if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
    sVar6 = -(sVar5 + sVar6);
  }
  sVar6 = *(short *)(unaff_A6 + 0x40) + sVar6;
  uVar1 = sVar4 + 0x20;
  if (sVar4 < -0x20) {
    bVar14 = SCARRY2(uVar1,sVar3);
    sVar3 = uVar1 + sVar3;
    if (bVar14 != sVar3 < 0) {
      return 0;
    }
    uVar1 = 0;
  }
  if ((short)uVar1 < 0x180) {
    sVar3 = uVar1 + sVar3;
    if (0x17f < sVar3) {
      sVar3 = 0x17f;
    }
    if ((ushort)(sVar3 - uVar1) != 0) {
      uVar2 = sVar6 - 0x108;
      if (sVar6 < 0x108) {
        bVar14 = SCARRY2(uVar2,sVar5);
        sVar5 = uVar2 + sVar5;
        if (bVar14 != sVar5 < 0) {
          return 0;
        }
        uVar2 = 0;
      }
      if ((short)uVar2 < 0xf0) {
        sVar5 = uVar2 + sVar5;
        if (0xef < sVar5) {
          sVar5 = 0xef;
        }
        if ((ushort)(sVar5 - uVar2) != 0) {
          sVar4 = ((ushort)(uVar1 + (sVar3 - uVar1)) >> 5) - (uVar1 >> 5);
          sVar6 = ((ushort)(uVar2 + (sVar5 - uVar2)) >> 5) - (uVar2 >> 5);
          puVar10 = (uint *)(iVar13 + 0x102 + (int)(short)((uVar1 >> 5) << 3));
          uVar8 = 0;
          uVar7 = 0;
          do {
            puVar9 = puVar10 + 1;
            uVar8 = *puVar10 | uVar8;
            puVar10 = puVar10 + 2;
            uVar7 = *puVar9 | uVar7;
            sVar4 = sVar4 + -1;
          } while (sVar4 != -1);
          *(uint *)(unaff_A5 + -0x18b8) = uVar7;
          *(uint *)(unaff_A5 + -0x18b4) = uVar8;
          if (uVar7 != 0 || uVar8 != 0) {
            puVar10 = (uint *)(iVar13 + 0x162 + (int)(short)((uVar2 >> 5) << 3));
            uVar8 = 0;
            do {
              puVar9 = puVar10 + 1;
              uVar8 = *puVar10 | uVar8;
              puVar10 = puVar10 + 2;
              uVar7 = *puVar9 | uVar7;
              sVar6 = sVar6 + -1;
            } while (sVar6 != -1);
            *(uint *)(unaff_A5 + -0x18b8) = uVar7 & *(uint *)(unaff_A5 + -0x18b8);
            *(uint *)(unaff_A5 + -0x18b4) = uVar8 & *(uint *)(unaff_A5 + -0x18b4);
            if (uVar7 != 0 || uVar8 != 0) {
              piVar11 = (int *)(in_A0 + 5);
              while( true ) {
                piVar12 = piVar11 + 1;
                if (*piVar11 < 0) break;
                if ((short)((uint)*piVar11 >> 0x10) == 0) {
                  puVar10 = (uint *)(iVar13 + 0x1a2);
                }
                else {
                  puVar10 = (uint *)(iVar13 + 0x1aa);
                }
                uVar8 = *puVar10 & *(uint *)(unaff_A5 + -0x18b4);
                uVar7 = puVar10[1] & *(uint *)(unaff_A5 + -0x18b8);
                if (uVar8 != 0) {
                  sVar4 = (short)uVar8;
                  while (uVar1 = (ushort)(uVar8 >> 0x10), sVar4 != 0) {
                    if (sVar4 < 0) {
                      candidate_4762();
                    }
                    sVar4 = (short)uVar8 * 2;
                    uVar8 = CONCAT22(uVar1,sVar4);
                  }
                  uVar8 = uVar8 >> 0x10;
                  while (uVar1 != 0) {
                    if ((short)uVar1 < 0) {
                      candidate_4762();
                    }
                    uVar1 = (short)uVar8 * 2;
                    uVar8 = (uint)uVar1;
                  }
                }
                piVar11 = piVar12;
                if (uVar7 != 0) {
                  sVar4 = (short)uVar7;
                  while (uVar1 = (ushort)(uVar7 >> 0x10), sVar4 != 0) {
                    if (sVar4 < 0) {
                      candidate_4762();
                    }
                    sVar4 = (short)uVar7 * 2;
                    uVar7 = CONCAT22(uVar1,sVar4);
                  }
                  uVar7 = uVar7 >> 0x10;
                  while (uVar1 != 0) {
                    if ((short)uVar1 < 0) {
                      candidate_4762();
                    }
                    uVar1 = (short)uVar7 * 2;
                    uVar7 = (uint)uVar1;
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
