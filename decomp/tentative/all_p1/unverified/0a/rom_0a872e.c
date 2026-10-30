/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a872e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a872e(void)

{
  byte bVar3;
  char cVar4;
  ushort uVar1;
  ushort uVar2;
  uint uVar5;
  short sVar6;
  undefined2 uVar7;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  uint6 uVar8;

  sVar6 = 5;
  do {
    candidate_40bc();
    *(short *)(extraout_A0 + 0x6e) = sVar6;
    sVar6 = sVar6 + -1;
  } while (sVar6 != -1);
  candidate_9ea8();
  candidate_9ea8();
  *(undefined2 *)(unaff_A6 + 0xf) = 0xa0;
  *(undefined2 *)(unaff_A6 + 0x10) = 0x180;
  candidate_4256();
  sVar6 = (*(byte *)((int)unaff_A6 + 0x6e) - 1) * 0x100;
  *(short *)((int)unaff_A6 + 0x72) = sVar6;
  *(short *)(unaff_A6 + 0x1d) = sVar6;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 900;
  candidate_9c0a();
  *(undefined2 *)((int)unaff_A6 + 0x76) = 8;
  *unaff_A6 = 0xa87bc;
  sVar6 = *(short *)((int)unaff_A6 + 0x76) + -1;
  *(short *)((int)unaff_A6 + 0x76) = sVar6;
  if (sVar6 == 0) {
    *unaff_A6 = 0xa87ca;
    if ((*(char *)(unaff_A5 + 0x7d82) == '\0') ||
       (sVar6 = *(short *)(unaff_A6 + 0x1e), *(short *)(unaff_A6 + 0x1e) = sVar6 + -1, sVar6 != 0))
    {
      uVar8 = candidate_190bc();
      uVar5 = (uint)uVar8;
      if ((uVar8 & 0x10) == 0) {
        uVar7 = 0xc;
        if ((*(short *)((int)unaff_A6 + 0x76) != 0) &&
           (sVar6 = *(short *)((int)unaff_A6 + 0x76) + -1, *(short *)((int)unaff_A6 + 0x76) = sVar6,
           sVar6 == 0)) {
          uVar7 = 6;
          uVar5 = (uint)(ushort)(uVar8 >> 0x20);
        }
        if ((uVar5 & 4) != 0) goto LAB_000a8856;
        if ((uVar5 & 8) != 0) {
          do {
            *(undefined2 *)((int)unaff_A6 + 0x76) = uVar7;
            bVar3 = *(char *)((int)unaff_A6 + 0x72) + 1;
            if (5 < bVar3) {
              bVar3 = *(char *)((int)unaff_A6 + 0x72) - 5;
            }
            *(byte *)((int)unaff_A6 + 0x72) = bVar3;
            *(undefined2 *)(unaff_A6 + 0x1c) = 1;
            while( true ) {
              if (*(byte *)((int)unaff_A6 + 0x72) < *(byte *)((int)unaff_A6 + 0x6e)) {
                candidate_120f8();
                goto LAB_000a888e;
              }
              if (-1 < *(short *)(unaff_A6 + 0x1c)) break;
LAB_000a8856:
              *(undefined2 *)((int)unaff_A6 + 0x76) = uVar7;
              cVar4 = *(char *)((int)unaff_A6 + 0x72) + -1;
              if (*(char *)((int)unaff_A6 + 0x72) == '\0') {
                cVar4 = '\x05';
              }
              *(char *)((int)unaff_A6 + 0x72) = cVar4;
              *(undefined2 *)(unaff_A6 + 0x1c) = 0xffff;
            }
          } while( true );
        }
LAB_000a888e:
        uVar1 = *(short *)((int)unaff_A6 + 0x72) - *(short *)(unaff_A6 + 0x1d);
        if (uVar1 != 0) {
          if (0x5ff < uVar1) {
            if ((short)(uVar1 - 0x600) < 0) {
              uVar1 = uVar1 + 0x600;
            }
            else {
              uVar1 = uVar1 - 0x600;
            }
          }
          if (*(short *)(unaff_A6 + 0x1c) < 0) {
            uVar1 = 0x600 - uVar1;
          }
          uVar2 = (uVar1 >> 4) + 0x20;
          if (uVar1 <= uVar2) {
            uVar2 = uVar1;
          }
          if (*(short *)(unaff_A6 + 0x1c) < 0) {
            uVar2 = -uVar2;
          }
          uVar2 = *(short *)(unaff_A6 + 0x1d) + uVar2;
          sVar6 = 0x600;
          if (0x5ff < uVar2) {
            if ((short)(uVar2 - 0x600) < 0) {
              sVar6 = -0x600;
            }
            uVar2 = uVar2 - sVar6;
          }
          *(ushort *)(unaff_A6 + 0x1d) = uVar2;
        }
      }
      else {
        candidate_120f8();
        *unaff_A6 = candidate_a88fc;
      }
    }
    else {
      *unaff_A6 = candidate_a88fc;
    }
  }
  candidate_427a();
  return;
}
