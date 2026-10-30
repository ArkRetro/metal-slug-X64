/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0693e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_693e4(void)

{
  char cVar1;
  short sVar3;
  int iVar2;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  candidate_69582();
  uVar4 = 0;
  *(ushort *)((int)unaff_A6 + 0x6e) = *(ushort *)((int)unaff_A6 + 0x6e) & 0xff7f;
  candidate_4e76();
  candidate_68bb8();
  if ((bool)uVar4) {
    candidate_68cae();
    if ((bool)uVar4) goto LAB_00067ee4;
    unaff_A6[0x14] = 0xe1a96;
    candidate_4e76();
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    candidate_690ec();
    *unaff_A6 = 0x67e98;
    uVar4 = 0;
    candidate_8ccd4();
    candidate_427a();
    if ((bool)uVar4) {
      candidate_68bb8();
      if ((bool)uVar4) {
        candidate_68cae();
        if ((bool)uVar4) {
          *unaff_A6 = 0x67ee4;
        }
        else {
          *unaff_A6 = 0x67e66;
        }
      }
      else {
        *unaff_A6 = 0x67ab4;
      }
      bVar5 = false;
      candidate_68aba();
      if (bVar5) {
        *unaff_A6 = 0x67ba6;
      }
    }
    goto LAB_00069280;
  }
  candidate_4256();
  unaff_A6[0x14] = 0xe1a96;
  uVar4 = 0;
  candidate_4e76();
  sVar3 = candidate_68a50();
  if ((bool)uVar4) goto LAB_00067ee4;
  candidate_1af9a();
  *(undefined4 *)((int)unaff_A6 + 0x86) = extraout_A0;
  if (*(short *)((int)unaff_A6 + 0x6e) != 2) {
    if (sVar3 == 0) {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
LAB_00067ba6:
        unaff_A6[0x14] = 0xe1a96;
        candidate_4e76();
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x67bd4;
        uVar4 = 0;
        candidate_8ccd4();
        candidate_427a();
        if ((bool)uVar4) {
          candidate_68bb8();
          if ((bool)uVar4) {
            candidate_68cae();
            if ((bool)uVar4) {
              *unaff_A6 = 0x67ee4;
            }
            else {
              *unaff_A6 = 0x67e66;
            }
          }
          else {
            *unaff_A6 = 0x67ab4;
          }
          bVar5 = false;
          candidate_68aba();
          if (bVar5) {
            *unaff_A6 = 0x67ba6;
          }
        }
        goto LAB_00069280;
      }
    }
    else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) goto LAB_00067ba6;
  }
  if (*(short *)((int)unaff_A6 + 0x6e) == 1) {
LAB_00067ee4:
    unaff_A6[0x14] = 0xe1a96;
    candidate_4e76();
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = 0x67f12;
    uVar4 = 0;
    candidate_8ccd4();
    candidate_427a();
    candidate_1af9a();
    if (!(bool)uVar4) {
      candidate_68cd4();
      if (!(bool)uVar4) {
        *unaff_A6 = 0x67ab4;
        uVar4 = 0;
      }
      candidate_68bb8();
      if ((bool)uVar4) {
        candidate_68cae();
        if ((bool)uVar4) {
          *unaff_A6 = 0x67f12;
          uVar4 = 0;
        }
        else {
          *unaff_A6 = 0x67e66;
          uVar4 = 0;
        }
      }
      candidate_68aba();
      if ((bool)uVar4) {
        *unaff_A6 = 0x67ba6;
        uVar4 = 0;
      }
    }
    candidate_4c24();
    if ((bool)uVar4) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        candidate_6914a();
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      candidate_411c();
      candidate_29c6();
      candidate_29c6();
      candidate_1acf2();
    }
    candidate_688d6();
    return;
  }
  iVar2 = *(int *)((int)unaff_A6 + 0x8a);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iVar2;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  *unaff_A6 = 0x67b42;
  uVar4 = 0;
  candidate_8ccd4();
  candidate_427a();
  candidate_1af9a();
  if ((bool)uVar4) {
    *unaff_A6 = 0x67ee4;
  }
  else {
    candidate_68cd4();
    if ((bool)uVar4) {
      *unaff_A6 = 0x67ee4;
      uVar4 = 0;
    }
    candidate_68bb8();
    if ((bool)uVar4) {
      candidate_68cae();
      if ((bool)uVar4) {
        *unaff_A6 = 0x67ee4;
        uVar4 = 0;
      }
      else {
        *unaff_A6 = 0x67e66;
        uVar4 = 0;
      }
    }
    candidate_68aba();
    if ((bool)uVar4) {
      *unaff_A6 = 0x67ba6;
    }
  }
  candidate_68b08();
LAB_00069280:
  bVar5 = false;
  if (*(char *)((int)unaff_A6 + 0x9a) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x9a);
    bVar5 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x9a) = cVar1 + -1;
  }
  candidate_4c24();
  if (bVar5) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_6914a();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_6918e();
  }
  candidate_688d6();
  return;
}
