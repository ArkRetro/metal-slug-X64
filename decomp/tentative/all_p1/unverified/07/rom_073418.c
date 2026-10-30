/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x073418.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_73418(void)

{
  byte bVar1;
  uint uVar2;
  short sVar3;
  char cVar4;
  undefined1 extraout_D1b;
  int iVar5;
  int iVar6;
  undefined4 *unaff_A6;
  undefined1 uVar7;
  bool bVar8;
  undefined8 uVar9;

  candidate_74c00();
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  unaff_A6[0xd] = 0x7c13b;
  *unaff_A6 = 0x73436;
  if (*(short *)(unaff_A6 + 0xd) < 0) {
    *unaff_A6 = 0x7345a;
  }
LAB_000734a2:
  do {
    uVar2 = candidate_74ce4();
    if ((uVar2 & 0x80) != 0) {
LAB_000736a2:
      candidate_aa7fc();
      candidate_74c00();
      candidate_73cd8();
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 4;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
      candidate_1b1dc();
      *(undefined1 *)((int)unaff_A6 + 0x97) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
      *unaff_A6 = 0x736e2;
      uVar7 = false;
      if ((*(short *)(unaff_A6 + 0xd) < 1) && (candidate_1945c(), (bool)uVar7)) {
        *unaff_A6 = 0x73ab0;
        uVar7 = 0;
      }
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        candidate_74c00();
        candidate_49e8();
        uVar9 = candidate_3b332();
        *(undefined8 *)(unaff_A6 + 0xc) = uVar9;
        *unaff_A6 = 0x73776;
        bVar8 = false;
        if ((*(short *)(unaff_A6 + 0xd) < 1) && (candidate_1945c(), bVar8)) {
          *unaff_A6 = 0x73ab0;
        }
        unaff_A6[0xd] = unaff_A6[0xd] + -0xa000;
        candidate_74b98();
        candidate_74c3c();
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
          *unaff_A6 = candidate_737d0;
        }
        sVar3 = candidate_49e8();
        if (sVar3 != 0) {
          *unaff_A6 = candidate_737d0;
        }
      }
      else {
        candidate_73cc0();
        candidate_1436a();
        candidate_74c3c();
        if ((bool)uVar7) {
          *unaff_A6 = 0x73728;
        }
      }
LAB_00073ca0:
      *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xbf;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x96) = 0xff;
      if (*(char *)(unaff_A6 + 0x27) != '\0') {
        *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
      }
      return;
    }
    if ((uVar2 >> 0x10 & 0x20) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
      *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xdf;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
    }
    if ((((*(byte *)((int)unaff_A6 + 0x95) & 0x20) == 0) || (-1 < *(short *)(unaff_A6 + 0xd))) ||
       (*(char *)(unaff_A6 + 0x25) == '\0')) {
      candidate_73cc0();
      if (*(char *)(unaff_A6 + 0x23) == '\x05') {
        *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
        candidate_74c00();
        *unaff_A6 = 0x7346a;
      }
      else {
        if (*(char *)((int)unaff_A6 + 0x91) != '\x03') {
          if ((*(byte *)((int)unaff_A6 + 0x8e) & 0x80) != 0) {
            candidate_74c00();
          }
LAB_00073524:
          if (*(char *)((int)unaff_A6 + 0x9d) == '\0') {
            unaff_A6[0xd] = unaff_A6[0xd] + -0xa000;
          }
          else {
            *(char *)((int)unaff_A6 + 0x9d) = *(char *)((int)unaff_A6 + 0x9d) + -1;
            unaff_A6[0xd] = (0x60000 - unaff_A6[0xd] >> 1) + unaff_A6[0xd];
          }
          uVar2 = candidate_74ce4();
          if ((uVar2 & 0x80) == 0) {
            iVar6 = 0;
            iVar5 = 5;
            if ((uVar2 & 4) != 0) {
              iVar6 = -0x40000;
              iVar5 = 2;
            }
            if ((uVar2 & 8) != 0) {
              iVar6 = 0x40000;
              iVar5 = 2;
            }
            unaff_A6[0xc] = (iVar6 - unaff_A6[0xc] >> iVar5) + unaff_A6[0xc];
            candidate_143ee();
            candidate_74c3c();
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
              *unaff_A6 = candidate_735ae;
            }
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
              *(undefined1 *)(unaff_A6 + 0x25) = 1;
              *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
              *(undefined1 *)((int)unaff_A6 + 0x9a) = 0x40;
            }
            *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xfb;
            cVar4 = candidate_49e8();
            if ((cVar4 == '\x05') || (cVar4 == '\x04')) {
              *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) | 4;
            }
            if (*(short *)((int)unaff_A6 + 0x82) != 0) {
              *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
              *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
            }
            bVar1 = *(byte *)((int)unaff_A6 + 0x5f);
            *(byte *)((int)unaff_A6 + 0x5f) = bVar1 & 0xbf;
            if ((bVar1 & 0x40) != 0) {
              *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
              *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
            }
            if (*(int *)((int)unaff_A6 + 0x6e) == 0) {
              *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
              if ((*(ushort *)((int)unaff_A6 + 0x82) & 1) != 0) {
                *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
              }
            }
            else {
              *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
              candidate_74be0();
              *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
            }
            if ((DAT_0010f000 & 0x80) != 0) {
              *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
            }
            bVar8 = false;
            candidate_aa760();
            if (bVar8) {
              cVar4 = candidate_aaa88();
              if (cVar4 == '\v') {
                *(undefined1 *)(unaff_A6 + 0x21) = extraout_D1b;
                *(undefined1 *)((int)unaff_A6 + 0x86) = 8;
              }
            }
            else {
              *unaff_A6 = candidate_72e86;
            }
            bVar8 = false;
            candidate_aa9e4();
            if (bVar8) {
              *(undefined2 *)((int)unaff_A6 + 0x82) = 0x1e;
              *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
            }
            if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
              *unaff_A6 = candidate_7387a;
            }
            goto LAB_00073ca0;
          }
          goto LAB_000736a2;
        }
        if (*(char *)(unaff_A6 + 0x23) != '\0') goto LAB_00073524;
        candidate_74c00();
        *unaff_A6 = 0x7349e;
      }
      goto LAB_000734a2;
    }
    *(byte *)((int)unaff_A6 + 0x95) = *(byte *)((int)unaff_A6 + 0x95) & 0xdf;
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
    *(char *)(unaff_A6 + 0x25) = *(char *)(unaff_A6 + 0x25) + -1;
    *(undefined1 *)((int)unaff_A6 + 0x9d) = 8;
    *unaff_A6 = 0x7348a;
  } while( true );
}
