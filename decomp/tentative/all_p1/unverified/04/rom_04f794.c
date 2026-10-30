/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04f794.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4f794(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    if (*(char *)(unaff_A6[3] + 0x60) == '\0') {
      *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    }
  }
  uVar3 = unaff_A6[0xc];
  uVar4 = CARRY4(*(uint *)((int)unaff_A6 + 0x7a),uVar3);
  unaff_A6[0xc] = *(uint *)((int)unaff_A6 + 0x7a) + uVar3;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_4f7e8();
  return;
}
