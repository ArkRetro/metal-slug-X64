/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06a992.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_6a992(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined1 *)((int)unaff_A6 + 0x95) = 4;
  uVar2 = 0;
  candidate_6bbe8();
  if ((*(byte *)((int)unaff_A6 + 0x92) & 1) == 0) {
    candidate_6b9e0();
    if ((bool)uVar2) {
      candidate_6bbe8();
      if ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0) {
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x6ac50;
        uVar2 = 0;
        candidate_6b7e0();
        candidate_427a();
        if ((bool)uVar2) {
          *unaff_A6 = 0x6ac68;
        }
      }
      else {
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x6ac82;
        uVar2 = 0;
        candidate_6b7e0();
        candidate_427a();
        if ((bool)uVar2) {
          *unaff_A6 = candidate_6a9e8;
        }
      }
      goto LAB_0006b222;
    }
    if ((*(byte *)((int)unaff_A6 + 0x92) & 4) == 0) {
      if ((*(byte *)((int)unaff_A6 + 0x92) & 8) == 0) {
                    /* WARNING: Could not recover jumptable at 0x0006a2d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*(code *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x95) << 2) + 0x6a2d4))();
        return;
      }
      if ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0) {
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x6aae0;
        uVar2 = 0;
        candidate_6b7e0();
        candidate_427a();
        if ((bool)uVar2) {
          *unaff_A6 = 0x6aaf8;
        }
      }
      else {
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x6ab12;
        uVar2 = 0;
        candidate_6b7e0();
        candidate_427a();
        if ((bool)uVar2) {
          *unaff_A6 = candidate_6ab2a;
        }
      }
      goto LAB_0006b222;
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x92) & 4) == 0) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if ((*(byte *)((int)unaff_A6 + 0x92) & 0x10) == 0) {
    if (*(char *)(unaff_A6 + 0x1b) == '\f') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = 0x6aaa4;
    uVar2 = 0;
    candidate_6b7e0();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_6a9e8;
    }
  }
  else {
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = candidate_6aa54;
    uVar2 = 0;
    candidate_6b7e0();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_6aa6c;
    }
  }
LAB_0006b222:
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_6accc;
  }
  if (*(short *)((int)unaff_A6 + 0x82) != 0) {
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  }
  if (*(char *)(unaff_A6 + 0x24) != '\0') {
    *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + -1;
  }
  uVar2 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x91);
    uVar2 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar1 + -1;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_6af44;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = 0x6b27e;
  }
  return;
}
