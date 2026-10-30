/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0751ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_751ec(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  iVar2 = unaff_A6[3];
  uVar3 = *(ushort *)((int)unaff_A6 + 0x7e);
  *(byte *)(iVar2 + 0x9d) = *(byte *)(iVar2 + 0x9d) & ~('\x01' << (uVar3 & 7));
  *(undefined2 *)(iVar2 + (short)(uVar3 * 2 + 0x86)) = *(undefined2 *)((int)unaff_A6 + 0x4e);
  *(short *)(iVar2 + 0x9a) = *(short *)(iVar2 + 0x9a) + -1;
  *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
  candidate_4192();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
