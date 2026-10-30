/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0438c8.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_438c8(void)

{
  code *pcVar1;
  undefined1 uVar4;
  ushort uVar2;
  undefined2 uVar3;
  char cVar5;
  int iVar6;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar7;

  candidate_1ad52();
  candidate_1abc8();
  candidate_4256();
  unaff_A6[0x14] = 0x44e1c;
  uVar4 = candidate_440de();
  *(undefined1 *)((int)unaff_A6 + 0x9b) = uVar4;
  if (*(char *)(unaff_A6 + 0x20) == '\x04') {
    candidate_4463e();
    uVar2 = candidate_260c8();
    if (3 < uVar2) {
      __m68k_trap(0xf);
      __m68k_trap(7);
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    bVar7 = false;
    candidate_44b84();
    if (bVar7) {
      iVar6 = 0x2c688c;
    }
    else {
      iVar6 = 0x2c4df2;
    }
    *(undefined2 *)((int)unaff_A6 + 0x96) = *(undefined2 *)(iVar6 + (short)(uVar2 * 4));
    *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(iVar6 + 2 + (int)(short)(uVar2 * 4));
    candidate_4463e();
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar3;
    candidate_4463e();
    uVar4 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x9d) = uVar4;
  }
  else {
    if (*(char *)(unaff_A6 + 0x20) == '\x05') {
      candidate_4463e();
      uVar4 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0x9d) = uVar4;
      goto LAB_00043ae4;
    }
    if (*(char *)(unaff_A6 + 0x20) == '\x06') {
      candidate_4463e();
      uVar2 = candidate_260c8();
      if (3 < uVar2) {
        __m68k_trap(0xf);
        __m68k_trap(7);
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      bVar7 = false;
      candidate_44b84();
      if (bVar7) {
        iVar6 = 0x2c7104;
      }
      else {
        iVar6 = 0x2c566a;
      }
      *(undefined2 *)((int)unaff_A6 + 0x96) = *(undefined2 *)(iVar6 + (short)(uVar2 * 4));
      *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(iVar6 + 2 + (int)(short)(uVar2 * 4));
      candidate_4463e();
      uVar3 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar3;
      candidate_4463e();
      uVar4 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0x9d) = uVar4;
    }
    else {
      candidate_4463e();
      uVar2 = candidate_260c8();
      if (3 < uVar2) {
        __m68k_trap(0xf);
        __m68k_trap(7);
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      bVar7 = false;
      candidate_44b84();
      if (bVar7) {
        iVar6 = 0x2c769e;
      }
      else {
        iVar6 = 0x2c5c04;
      }
      *(undefined2 *)((int)unaff_A6 + 0x96) = *(undefined2 *)(iVar6 + (short)(uVar2 * 4));
      *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(iVar6 + 2 + (int)(short)(uVar2 * 4));
      candidate_4463e();
      uVar3 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar3;
      candidate_4463e();
      uVar4 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0x9d) = uVar4;
    }
  }
  candidate_43daa();
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0x80;
  bVar7 = false;
  candidate_1aeb0();
  if (bVar7) {
    candidate_43c74();
  }
LAB_00043ae4:
  candidate_4463e();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar3;
  cVar5 = candidate_29c6();
  if (cVar5 < '\0') {
    *(byte *)((int)unaff_A6 + 0x9f) = *(byte *)((int)unaff_A6 + 0x9f) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x9f) = *(byte *)((int)unaff_A6 + 0x9f) | 1;
  }
  *unaff_A6 = 0x43b1c;
  bVar7 = false;
  candidate_43b68();
  if (bVar7) {
    *unaff_A6 = candidate_43e36;
  }
  bVar7 = false;
  candidate_1aeb0();
  if (bVar7) {
    uVar4 = *(byte *)(unaff_A6 + 0x20) < 5;
    if (*(byte *)(unaff_A6 + 0x20) == 5) {
      candidate_43bc0();
    }
    else {
      candidate_43c32();
    }
  }
  else {
    *unaff_A6 = candidate_43ee0;
    uVar4 = 0;
  }
  candidate_1ac38();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar4) {
    candidate_4192();
    candidate_4126();
    iVar6 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar6 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) | 6;
    if (*(char *)((int)unaff_A6 + 0x5a) == '\f') {
      *unaff_A6 = candidate_43f32;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_43f32;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if (*(char *)(unaff_A6 + 0x27) != '\0') {
    *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
  }
  candidate_446f0();
  return;
}
