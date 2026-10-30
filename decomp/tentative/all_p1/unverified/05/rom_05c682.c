/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05c682.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5c682(void)

{
  short sVar1;
  char cVar2;
  short sVar3;
  byte bVar5;
  ushort uVar4;
  undefined4 extraout_A0;
  undefined4 extraout_A0_00;
  undefined4 *unaff_A6;
  undefined1 uVar6;
  bool bVar7;

  unaff_A6[0x14] = 0x5e918;
  candidate_1b04a();
  *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  candidate_5e720();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 1;
  candidate_4256();
  *unaff_A6 = 0x5c6bc;
  uVar6 = 0;
  candidate_5e234();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x93) & 0x10) == 0) {
    candidate_5e5ba();
    if ((bool)uVar6) {
      *unaff_A6 = candidate_5cc94;
      uVar6 = 0;
    }
  }
  else {
    bVar7 = false;
    if (*(char *)((int)unaff_A6 + 0x91) == '\0') {
      candidate_1aeda();
      if (bVar7) {
        bVar5 = candidate_aaa88();
        uVar6 = bVar5 < 4;
        if (bVar5 != 4) {
          uVar4 = candidate_5e2a0();
          if (uVar4 == 1) {
            *unaff_A6 = candidate_5c870;
            uVar6 = false;
          }
          else {
            uVar6 = uVar4 < 2;
            if (uVar4 == 2) {
              *unaff_A6 = 0x5c91c;
              uVar6 = false;
            }
          }
        }
        candidate_5e4a8();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_5c7e8;
          uVar6 = 0;
        }
        candidate_5e65e();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_5cb06;
          uVar6 = 0;
        }
        candidate_5e5ba();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_5cc94;
          uVar6 = 0;
        }
        candidate_5e4f0();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_5c9c0;
          uVar6 = 0;
        }
        candidate_5e536();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_5cb72;
          uVar6 = 0;
        }
      }
      else {
        candidate_1b04a();
        *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0_00;
        uVar6 = 0;
      }
    }
    else {
      cVar2 = *(char *)((int)unaff_A6 + 0x91);
      uVar6 = cVar2 == '\0';
      *(char *)((int)unaff_A6 + 0x91) = cVar2 + -1;
      candidate_5e5ba();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_5cc94;
        uVar6 = 0;
      }
    }
  }
  candidate_5e30a();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_5cd90;
    uVar6 = 0;
  }
  candidate_5e244();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_5cd08;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar3 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar3;
  if (-1 >= sVar3) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar7 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar7) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar7 = bVar7 && (-1 < sVar3 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar7) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar7 = false;
  candidate_1945c();
  if (bVar7) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
